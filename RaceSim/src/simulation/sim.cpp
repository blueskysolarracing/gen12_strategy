#include <sim.h>
#include <stdlib.h>
#include <assert.h>
#include <units.h>
#include <unordered_set>
#include <string.h>
#include <route.h>
#include <config.h>
#include <geography.h>

bool Sim::run_sim(Route route, std::vector<uint32_t> speed_profile_kph) {
    assert(speed_profile_kph.size() == route.get_num_segments());

    /* Reset variables for each simulation */
    double max_soc = Config::get_instance()->get_max_soc();
    Coord starting_coord = Config::get_instance()->get_gps_coordinates();
    battery_energy = max_soc;
    curr_time = *Config::get_instance()->get_current_date_time();
    wind_speed_lut.initialize_caches(starting_coord, curr_time.get_utc_time_point());
    wind_dir_lut.initialize_caches(starting_coord, curr_time.get_utc_time_point());
    dni_lut.initialize_caches(starting_coord, curr_time.get_utc_time_point());
    dhi_lut.initialize_caches(starting_coord, curr_time.get_utc_time_point());
    total_distance = 0.0;

    uint32_t num_points = route.get_num_points();
    std::vector<Coord> route_points = route.get_route_points();
    std::vector<std::pair<uint32_t, uint32_t>> segments = route.get_segments();
    std::unordered_set<uint32_t> control_stops = route.get_control_stops();

    assert(segments.size() == speed_profile_kph.size());

    uint32_t segment_counter = 0;
    std::pair<uint32_t, uint32_t> current_segment = segments[segment_counter];
    double current_speed = kph2mps(speed_profile_kph[segment_counter]);

    /* Get starting position in the route */
    size_t starting_route_index = 0;
	double min_distance = std::numeric_limits<double>::max();
	for (size_t i = 0; i < num_points; i++) {
		Coord route_coord = route_points[i];
		double distance = get_distance(route_coord, starting_coord);
		if (distance < min_distance) {
			min_distance = distance;
			starting_route_index = i;
		}
	}

    for (size_t idx=starting_route_index; idx<num_points-1; idx++) {
        Coord coord_one = route_points[idx];
        Coord coord_two = route_points[idx+1];
        double energy_change = 0.0;

        /* Get the position of the sun in the sky */
        SolarAngle sun_position = SolarAngle();

        /* Get forecasting data - wind and irradiance */
        ForecastCoord coord_one_forecast = ForecastCoord(coord_one.lat, coord_one.lon);

        wind_speed_lut.update_index_cache(coord_one_forecast, curr_time.get_utc_time_point());
        double wind_speed = wind_speed_lut.get_value_with_cache();

        wind_dir_lut.update_index_cache(coord_one_forecast, curr_time.get_utc_time_point());
        double wind_dir = wind_dir_lut.get_value_with_cache();

        dni_lut.update_index_cache(coord_one_forecast, curr_time.get_utc_time_point());
        double dni = dni_lut.get_value_with_cache();

        dhi_lut.update_index_cache(coord_one_forecast, curr_time.get_utc_time_point());
        double dhi = dhi_lut.get_value_with_cache();

        Wind wind = Wind(wind_dir, wind_speed);
        Irradiance irr = Irradiance(dni, dhi);

        /* Overnight stop */
        while (curr_time.get_local_hours() > race_end || curr_time.get_local_hours() < race_start) {
            /* Step in 30 second intervals */
            SolarAngle sun = SolarAngle();
            get_az_el(curr_time.get_utc_time_point(), coord_one.lat, coord_one.lon, coord_one.alt, &sun.Az, &sun.El);

            if (sun.El > 0) {
                /* Charging at dawn/dusk */
                energy_change += car->compute_static_energy(coord_one, curr_time, OVERNIGHT_STEP_SIZE, irr);
                curr_time.update_time_seconds(OVERNIGHT_STEP_SIZE);
                dni_lut.update_index_cache(coord_one_forecast, curr_time.get_utc_time_point());
                double dni = dni_lut.get_value_with_cache();

                dhi_lut.update_index_cache(coord_one_forecast, curr_time.get_utc_time_point());
                double dhi = dhi_lut.get_value_with_cache();
                irr = Irradiance(dni, dhi);
            } else {
                curr_time.update_time_seconds(OVERNIGHT_STEP_SIZE);
            }

        }

        /* Control Stop */
        if (control_stops.find(idx) != control_stops.end()) {
            energy_change += car->compute_static_energy(coord_one, curr_time, control_stop_charge_time, irr);
        }

        /* Move from point one to point two */
        if (idx > current_segment.second) {
            segment_counter++;
            current_segment = segments[segment_counter];
            current_speed = kph2mps(speed_profile_kph[segment_counter]);
        }

        /* Compute distance travelled */
        double distance_to_travel = get_distance(coord_one, coord_two);
        double delta_time = distance_to_travel / current_speed;
        total_distance += distance_to_travel;

        /* Compute energy change between two points */
        energy_change += car->compute_travel_energy(coord_one, coord_two, current_speed, delta_time, curr_time, wind, irr);

        /* Update the time */
        curr_time.update_time_seconds(delta_time);

        if (battery_energy + energy_change > max_soc) {
            battery_energy = max_soc;
        } else {
            battery_energy += energy_change;
        }

        /* Penalize a minimum soc bound TODO */
        if (battery_energy < 0.0) {
            return false;
        }
    }

    return true;
}

Sim::Sim(Car* model) : 
    car(model),
    wind_speed_lut(Forecast_Lut(Config::get_instance()->get_wind_speed_path())),
    wind_dir_lut(Forecast_Lut(Config::get_instance()->get_wind_dir_path())),
    dni_lut(Forecast_Lut(Config::get_instance()->get_dni_path())),
    dhi_lut(Forecast_Lut(Config::get_instance()->get_dhi_path())),
    control_stop_charge_time(Config::get_instance()->get_control_stop_charge_time()),
    race_start(Config::get_instance()->get_day_start()),
    race_end(Config::get_instance()->get_day_end()),
    starting_coord(Config::get_instance()->get_gps_coordinates()),
    curr_time(*Config::get_instance()->get_current_date_time())
    {
}