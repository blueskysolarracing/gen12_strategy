/*
Base class for force balance car models
*/

#ifndef BASE_CAR_H
#define BASE_CAR_H

#include <units.h>
#include <Luts.h>

class Base_Car {
protected:
    /* Car parameters */
    double mass;
    Eff_Lut yint_rolling_resistance;
    Eff_Lut slope_rolling_resistance;
    Eff_Lut power_factors;
    double cda;
    double motor_efficiency;
    double regen_efficiency;
    double battery_efficiency; 
    double passive_electric_loss;
    double air_density;
    double array_area;
    double max_soc;
    double tire_pressure;
    double array_efficiency;

public:

    /* Constructor */
    Base_Car();

    /* Compute the aerodynamic loss */
    virtual Energy_Change aero_loss(double speed, double car_bearing, Wind wind, double delta_time_s) = 0;

    /* Compute the rolling resistance loss */
    virtual Energy_Change rolling_loss(double speed, double delta_time) = 0;

    /* Compute the gravitational loss */
    virtual Energy_Change gravitational_loss(double delta_altitude, double delta_time) = 0;

    /* Compute electric loss */
    virtual double electric_loss(double delta_time) = 0;

    /* Compute the array energy gains */
    virtual Energy_Change array_gain(double delta_time, double dni, double dhi, double az, double el) = 0;

    /* Compute the net battery energy change */
    virtual double net_battery_change(
        double array, double aero, double rolling, double gravity, double electric, double motor
    ) = 0;

    /* Compute energy change when moving between two points in a straight line */
    virtual double compute_travel_energy(Coord coord_one, Coord coord_two, uint32_t speed) = 0;

    /* Compute energy change during a static stop */
    virtual double compute_static_energy(Coord coord, Time time) = 0;

    /* Getters */
    double get_mass() const;
    double get_cda() const;
    Eff_Lut get_yint_rr() const;
    Eff_Lut get_slope_rr() const;
    double get_motor_efficiency() const;
    double get_battery_efficiency() const;
    double get_regen_efficiency() const;
    double get_passive_electric_loss() const;
    double get_air_density() const;
    double get_array_area() const;
    double get_max_soc() const;

    /* Setters */
    void set_mass(double new_mass);
    void set_cda(double new_cda);
    void set_yint_rr(Eff_Lut new_yint_rr);
    void set_slope_rr(Eff_Lut new_slope_rr);
    void set_motor_efficiency(double new_motor_efficiency);
    void set_battery_efficiency(double new_battery_efficiency);
    void set_regen_efficiency(double new_regen_efficiency);
    void set_passive_electric_loss(double new_passive_electric_loss);
    void set_air_density(double new_air_density);
    void set_array_area(double new_array_area);
    void set_max_soc(double new_max_soc);
};

#endif
