/*
Base class for force balance car models
*/

#ifndef BASE_CAR_H
#define BASE_CAR_H

#include <units.h>
#include <Luts.h>
#include <config.h>

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
    virtual Energy_Change compute_aero_loss(double speed, double car_bearing, Wind wind, double delta_time_s) = 0;

    /* Compute the rolling resistance loss */
    virtual Energy_Change compute_rolling_loss(double speed, double delta_time) = 0;

    /* Compute the gravitational loss */
    virtual Energy_Change compute_gravitational_loss(double delta_altitude, double delta_time) = 0;

    /* Compute electric loss */
    virtual double compute_electric_loss(double delta_time) = 0;

    /* Compute the array energy gains */
    virtual Energy_Change compute_array_gain(double delta_time, double dni, double dhi, double az, double el) = 0;

    /* Compute energy change when moving between two points in a straight line */
    virtual double compute_travel_energy(Coord coord_one, Coord coord_two, uint32_t speed, Time time, Wind wind, Irradiance irr) = 0;

    /* Compute energy change during a static stop */
    virtual double compute_static_energy(Coord coord, Time time, double charge_time, Irradiance irr) = 0;
};

#endif
