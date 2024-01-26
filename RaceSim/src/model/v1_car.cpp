/*
Model of the car implemented for gen 11.5
*/

#include <v1_car.h>
#include <math.h>
#include <units.h>
#include <geography.h>

Energy_Change V1_Car::compute_aero_loss(double speed, double car_bearing, Wind wind, double delta_time) {
    double speed_relative_to_wind = get_speed_relative_to_wind(speed, car_bearing, wind);
    double power = (0.5 * air_density * cda * pow(speed_relative_to_wind, 2) * speed);
    double energy = watts2kwh(delta_time, power);

    return Energy_Change(power, energy);
}

Energy_Change V1_Car::compute_rolling_loss(double speed, double delta_time) {
    double y_int_rr = yint_rolling_resistance.get_value(tire_pressure, mps2kph(speed));
    double slope_rr = slope_rolling_resistance.get_value(tire_pressure, mps2kph(speed));

    double force_scaling = (y_int_rr + slope_rr * mps2kph(speed));
    double normal_force = mass * GRAVITY_ACCELERATION;
    double power = force_scaling * normal_force * speed;
    double energy = watts2kwh(delta_time, power);

    return Energy_Change(power, energy);
}

Energy_Change V1_Car::compute_gravitational_loss(double delta_altitude, double delta_time) {
    double power = (mass * GRAVITY_ACCELERATION * delta_altitude) / delta_time;
    double energy = watts2kwh(delta_time, power);

    return Energy_Change(power, energy);
}

double V1_Car::compute_electric_loss(double delta_time) {
    return joules2kwh(delta_time * passive_electric_loss);
}

Energy_Change V1_Car::compute_array_gain(double delta_time, double dni, double dhi, double az, double el) {
    double power_factor = power_factors.get_value(round(el), round(az));
    double power = (power_factor * dni) + (dhi * array_efficiency * array_area);
    double energy = watts2kwh(delta_time, power);

    return Energy_Change(power,energy);
}

double V1_Car::compute_net_battery_change(
    double array, double aero, double rolling, double gravity, double electric, double motor
) {
	double beta = aero + rolling + gravity > 0 ? 1 / motor_efficiency : regen_efficiency; // Use regen or not
	double alpha = array > beta * motor + electric ? battery_efficiency : 1 / battery_efficiency; // Charging or discharging
	double delta_battery_energy = alpha * (array - beta * motor - electric);

	return delta_battery_energy;
}

Car_Update V1_Car::compute_travel_update(
    Coord coord_one, 
    Coord coord_two, 
    double speed,
    Time time, 
    Wind wind, 
    Irradiance irr
) {
    /* Get orientation of the car */
    double bearing = get_bearing(coord_one, coord_two);
    SolarAngle az_el = get_az_el_from_bearing(bearing, coord_one, time);
    if (az_el.El < 0) {
        az_el.El = 0.0;
    }

    /* Get time and distance travelled. This is a redundant calculation as sim calculates these values already.
    For the sake of readability, we calculate them again so that this function takes in only the necessary parameters */
    double delta_distance = get_distance(coord_one, coord_two);
    double delta_time = delta_distance / speed;

    /* Calculate energy losses */
    double electric_loss = compute_electric_loss(delta_time);
    double motor_loss = 0.0;
    Energy_Change aero_loss;
    Energy_Change rolling_loss;
    Energy_Change gravity_loss;
    double delta_altitude = coord_two.alt - coord_one.alt;

	while (speed > 0) {
		aero_loss = compute_aero_loss(speed, bearing, wind, delta_time);
		rolling_loss = compute_rolling_loss(speed, delta_time);
		gravity_loss = compute_gravitational_loss(delta_altitude, delta_time);

		motor_loss = aero_loss.energy + rolling_loss.energy + gravity_loss.energy;
		if (motor_loss < max_power) {
			break;
		}
		speed--;
	}

    Energy_Change array_gain = compute_array_gain(delta_time, irr.dni, irr.dhi, az_el.Az, az_el.El);
    double delta_battery = compute_net_battery_change(array_gain.energy, aero_loss.energy, rolling_loss.energy, gravity_loss.energy, electric_loss, motor_loss);

    return Car_Update(delta_battery, delta_distance, delta_time);
}

double V1_Car::compute_static_energy(Coord coord, Time time, double charge_time, Irradiance irr) {
    /* Get orientation of the car */
    SolarAngle sun = SolarAngle();
    get_az_el(time.get_utc_time_point(), coord.lat, coord.lon, coord.alt, &sun.Az, &sun.El);
    if (sun.El < 0) {
        sun.El = 0.0;
    } 

    double electric_loss = compute_electric_loss(charge_time);
    Energy_Change array_gain = compute_array_gain(charge_time, irr.dni, irr.dhi, sun.Az, sun.El);

	double delta_battery = array_gain.energy > electric_loss ? battery_efficiency * (array_gain.energy - electric_loss) : (1 / battery_efficiency) * (array_gain.energy - electric_loss);

    return delta_battery;
}

V1_Car::V1_Car() : Car() { max_power = Config::get_instance()->get_max_motor_power();}
