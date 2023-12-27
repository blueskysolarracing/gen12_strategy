/*
Base class for force balance car models
*/

#ifndef BASE_CAR_H
#define BASE_CAR_H

#include <units.h>

class Base_Car {
protected:
    /* Car parameters */
    double mass;
    double rr;
    double cda;
    double motor_efficiency;
    double regen_efficiency;
    double battery_efficiency; 
    double passive_electric_loss;
    double air_density;
    double array_area;
    double max_soc;

public:
    /* Compute the aerodynamic loss */
    virtual double aero_loss() = 0;

    /* Compute the rolling resistance loss */
    virtual double rolling_loss() = 0;

    /* Compute the gravitational loss */
    virtual double gravitational_loss() = 0;

    /* Compute electric loss */
    virtual double electric_loss() = 0;

    /* Compute the array energy gains */
    virtual double array_gain() = 0;

    /* Compute the net battery energy change */
    virtual double net_battery_change() = 0;

    /* Compute energy change when moving between two points in a straight line */
    virtual double compute_travel_energy(Coord coord_one, Coord coord_two, uint32_t speed) = 0;

    /* Compute energy change during a static stop */
    virtual double compute_static_energy(Coord coord, Time time) = 0;

    /* Getters */
    double get_mass() const;
    double get_cda() const;
    double get_rr() const;
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
    void set_rr(double new_rr);
    void set_motor_efficiency(double new_motor_efficiency);
    void set_battery_efficiency(double new_battery_efficiency);
    void set_regen_efficiency(double new_regen_efficiency);
    void set_passive_electric_loss(double new_passive_electric_loss);
    void set_air_density(double new_air_density);
    void set_array_area(double new_array_area);
    void set_max_soc(double new_max_soc);
};

#endif
