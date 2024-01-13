#include <sim.h>
#include <stdlib.h>
#include <assert.h>
#include <units.h>
#include <unordered_set>
#include <string.h>
#include <config.h>

bool Sim::run_sim(Route route, Base_Car* car, std::vector<uint32_t> speed_profile) {
    assert(speed_profile.size() == route.get_num_segments());

    uint32_t num_points = route.get_num_points();
    std::vector<Coord> route_points = route.get_route_points();
    std::vector<std::pair<uint32_t, uint32_t>> segments = route.get_segments();
    std::unordered_set<uint32_t> control_stops = route.get_control_stops();

    assert(segments.size() == speed_profile.size());

    uint32_t segment_counter = 0;
    std::pair<uint32_t, uint32_t> current_segment = segments[segment_counter];
    uint32_t current_speed = speed_profile[segment_counter];

    /* Todo, make this start at some set time and update it during the loop */
    Time curr_time = Time();

    for (size_t idx=0; idx<num_points-1; idx++) {
        Coord coord_one = route_points[idx];
        Coord coord_two = route_points[idx+1];
        double energy_change = 0.0;

        /* Control Stop */
        if (control_stops.find(idx) != control_stops.end()) {
            //energy_change += car->compute_static_energy(coord_one, curr_time);
        }

        /* Move from point one to point two */
        if (idx > current_segment.second) {
            segment_counter++;
            current_segment = segments[segment_counter];
            current_speed = speed_profile[segment_counter];
        }

        /* Get forecasting data - wind and irradiance */
        double wind_speed = wind_speed_lut.get_value(coord_one, curr_time.get_utc_time_point());
        double wind_dir = wind_dir_lut.get_value(coord_one, curr_time.get_utc_time_point());
        double dni = dni_lut.get_value(coord_one, curr_time.get_utc_time_point());
        double dhi = dhi_lut.get_value(coord_one, curr_time.get_utc_time_point());

        Wind wind = Wind(wind_dir, wind_speed);
        Irradiance irr = Irradiance(dni, dhi);

        energy_change += car->compute_travel_energy(coord_one, coord_two, current_speed, curr_time, wind, irr);

        battery_energy += energy_change;

        /* Penalize a minimum soc bound TODO */
        if (battery_energy < 0.0) {
            return false;
        }
    }

    return true;
}

Sim::Sim() {
    Config* params = Config::get_instance();

    wind_speed_lut = Forecast_Lut(params->get_wind_speed_path());
    wind_dir_lut = Forecast_Lut(params->get_wind_dir_path());
    dni_lut = Forecast_Lut(params->get_dni_path());
    dhi_lut = Forecast_Lut(params->get_dhi_path());
    battery_energy = params->get_max_soc();
}