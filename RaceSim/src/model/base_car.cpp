#include <base_car.h>

/* Getters */
double Base_Car::get_mass() const {return mass;}
double Base_Car::get_cda() const {return cda;}
double Base_Car::get_rr() const {return rr;}
double Base_Car::get_motor_efficiency() const {return motor_efficiency;}
double Base_Car::get_battery_efficiency() const {return battery_efficiency;}
double Base_Car::get_regen_efficiency() const {return regen_efficiency;}
double Base_Car::get_passive_electric_loss() const {return passive_electric_loss;}
double Base_Car::get_array_area() const {return array_area;}
double Base_Car::get_air_density() const {return air_density;}
double Base_Car::get_max_soc() const {return max_soc;}

/* Setters */
void Base_Car::set_mass(double new_mass) {mass = new_mass;}
void Base_Car::set_cda(double new_cda) {cda = new_cda;}
void Base_Car::set_rr(double new_rr) {rr = new_rr;}
void Base_Car::set_motor_efficiency(double new_motor_efficiency) {motor_efficiency = new_motor_efficiency;}
void Base_Car::set_battery_efficiency(double new_battery_efficiency) {battery_efficiency = new_battery_efficiency;}
void Base_Car::set_regen_efficiency(double new_regen_efficiency) {regen_efficiency = new_regen_efficiency;}
void Base_Car::set_passive_electric_loss(double new_passive_electric_loss) {passive_electric_loss = new_passive_electric_loss;}
void Base_Car::set_array_area(double new_array_area) {array_area = new_array_area;}
void Base_Car::set_air_density(double new_air_density) {air_density = new_air_density;}
void Base_Car::set_max_soc(double new_max_soc) {max_soc = new_max_soc;}
