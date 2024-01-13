/*
Model of the car implemented for gen 11.5
*/

#ifndef V1_Car
#define V1_CAR

#include <base_car.h>
#include <units.h>

class V1_Car : Base_Car {
private:
    double max_power;

    /* Compute the net battery energy change */
    double compute_net_battery_change(
        double array, double aero, double rolling, double gravity, double electric, double motor
    );

public:

    /* Compute the aerodynamic loss */
    Energy_Change compute_aero_loss(double speed, double car_bearing, Wind wind, double delta_time) override;

    /* Compute the rolling resistance loss */
    Energy_Change compute_rolling_loss(double speed, double delta_time) override;

    /* Compute the gravitational loss */
    Energy_Change compute_gravitational_loss(double delta_altitude, double delta_time) override;

    /* Compute electric loss */
    double compute_electric_loss(double delta_time) override;

    /* Compute the array energy gains */
    Energy_Change compute_array_gain(double delta_time, double dni, double dhi, double az, double el) override;

    /* Compute energy change when moving between two points in a straight line */
    double compute_travel_energy(Coord coord_one, Coord coord_two, uint32_t speed, Time time, Wind wind, Irradiance irr) override;

    /* Compute energy change during a static stop */
    double compute_static_energy(Coord coord, Time time, double charge_time, Irradiance irr) override;
};

#endif 
