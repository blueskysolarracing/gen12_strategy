/*
Base class for force balance car models
*/

#ifndef CAR_H
#define CAR_H

#include <units.h>
#include <Luts.h>
#include <config.h>

struct Car_Update {
    double delta_energy;
    double delta_distance;
    double delta_time;

    Car_Update(double e, double d, double t) : delta_energy(e), delta_distance(d), delta_time(t) {}
};

class Car {
protected:
    /* Car parameters */
    double mass;
    Eff_Lut yint_rolling_resistance;
    Eff_Lut slope_rolling_resistance;
    Basic_Lut power_factors;
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
    Car();

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

    /* Compute energy change when moving between two points in a straight line 
       @param coord_one: starting coordinate
       @param coord_two: ending coordinate
       @param speed: speed of the car
       @param time: current time at coord_one
       @param Wind, Irradiance: Weather forecast taken at coord_one
       @return Car_Update: State updates to the car after moving from coord_one to coord_two
    */
    virtual Car_Update compute_travel_update(Coord coord_one, 
                                        Coord coord_two, 
                                        double speed, 
                                        Time time, 
                                        Wind wind, 
                                        Irradiance irr) = 0;

    /* Compute energy change during a static stop */
    virtual double compute_static_energy(Coord coord, Time time, double charge_time, Irradiance irr) = 0;
};

#endif
