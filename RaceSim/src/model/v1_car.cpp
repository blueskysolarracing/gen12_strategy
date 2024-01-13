/*
Model of the car implemented for gen 11.5
*/

#include <v1_car.h>
#include <math.h>
#include <units.h>

Energy_Change V1_Car::aero_loss(double speed, double car_bearing, Wind wind, double delta_time) {
    double speed_relative_to_wind = get_speed_relative_to_wind(speed, car_bearing, wind);
    double distance_travelled = delta_time * speed;
    double power = (0.5 * air_density * cda * pow(speed_relative_to_wind, 2) * speed);
    double energy = watts2kwh(delta_time, power);

    return Energy_Change(power, energy);
}

Energy_Change V1_Car::rolling_loss(double speed, double delta_time) {
    double y_int_rr = yint_rolling_resistance.get_value(tire_pressure, speed);
    double slope_rr = slope_rolling_resistance.get_value(tire_pressure, speed);
    double distance_travelled = delta_time * speed;

    double normal_force = mass * GRAVITY_ACCELERATION;
    double power = normal_force * speed;
    double energy = watts2kwh(delta_time, power);

    return Energy_Change(power, energy);
}

Energy_Change V1_Car::gravitational_loss(double delta_altitude, double delta_time) {
    double power = (mass * GRAVITY_ACCELERATION * delta_altitude) / delta_time;
    double energy = watts2kwh(delta_time, power);

    return Energy_Change(power, energy);
}

double V1_Car::electric_loss(double delta_time) {
    return joules2kwh(delta_time * passive_electric_loss);
}

Energy_Change V1_Car::array_gain(double delta_time, double dni, double dhi, double az, double el) {
    double power_factor = power_factors.get_value(round(az), round(el));
    double power = (power_factor * dni) + (dhi * array_efficiency * array_area);
    double energy = watts2kwh(delta_time, power);

    return Energy_Change(power,energy);
}

double V1_Car::net_battery_change(
    double array, double aero, double rolling, double gravity, double electric, double motor
) {
	double beta = aero + rolling + gravity > 0 ? 1 / motor_efficiency : regen_efficiency; // Use regen or not

	double alpha = array > beta * motor + electric ? battery_efficiency : 1 / battery_efficiency; // Charging or discharging

	double delta_battery_energy = alpha * (array - beta * motor - electric);

	return delta_battery_energy;
}

double V1_Car::compute_travel_energy(Coord coord_one, Coord coord_two, uint32_t speed) {
    return 0.0;
}

double V1_Car::compute_static_energy(Coord coord, Time time) {
    return 0.0;
}

