#include <base_car.h>
#include <Luts.h>
#include <config.h>

/* Load all LUTs and car parameters */
Car::Car() : 
    yint_rolling_resistance((Config::get_instance()->get_roll_res_yint_path())),
    slope_rolling_resistance(Config::get_instance()->get_roll_res_slope_path()),
    power_factors(Config::get_instance()->get_power_factor_path()),
    mass(Config::get_instance()->get_car_mass()),
    cda(Config::get_instance()->get_cda()),
    motor_efficiency(Config::get_instance()->get_motor_efficiency()),
    regen_efficiency(Config::get_instance()->get_regen_efficiency()),
    battery_efficiency(Config::get_instance()->get_battery_efficiency()),
    passive_electric_loss(Config::get_instance()->get_passive_electric_loss()),
    air_density(Config::get_instance()->get_air_density()),
    array_area(Config::get_instance()->get_array_area()),
    max_soc(Config::get_instance()->get_max_soc()),
    tire_pressure(Config::get_instance()->get_tire_pressure()),
    array_efficiency(Config::get_instance()->get_array_efficiency())
    {
}
