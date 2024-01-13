#include <base_car.h>
#include <Luts.h>
#include <config.h>

/* Load all LUTs and car parameters */
Base_Car::Base_Car() {
    Config* param = Config::get_instance();
    yint_rolling_resistance = Eff_Lut(param->get_roll_res_yint_path());
    slope_rolling_resistance = Eff_Lut(param->get_roll_res_slope_path());
    power_factors = Eff_Lut(param->get_power_factor_path());
    mass = param->get_car_mass();
    cda = param->get_cda();
    motor_efficiency = param->get_motor_efficiency();
    regen_efficiency = param->get_regen_efficiency();
    battery_efficiency = param->get_battery_efficiency();
    passive_electric_loss = param->get_passive_elec_loss();
    air_density = param->get_air_density();
    array_area = param->get_array_area();
    max_soc = param->get_max_soc();
    tire_pressure = param->get_tire_pressure();
    array_efficiency = param->get_array_eff();
}
