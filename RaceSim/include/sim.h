/* 
Functions to simulate the car traversing a designated route
*/

#ifndef SIM_H
#define SIM_H

#include <route.h>
#include <base_car.h>
#include <stdbool.h>

/* Run a full simulation with a car object and a route. Return true if viable 
   @param route: The Route to simulate on
   @param car: The instance of a car to use
   @param speed_profile: The speed to use for each segment
   
   TODO: Add parameters for current time, maximum soc bound and minimum soc bound as a percentage
*/
bool run_sim(Route route, Base_Car* car, std::vector<uint32_t> speed_profile);

#endif 