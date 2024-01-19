/* 
Class to run the a full scale simulation on a designated route
*/

#ifndef SIM_H
#define SIM_H

#include <route.h>
#include <base_car.h>
#include <stdbool.h>
#include <units.h>
#include <Luts.h>
#include <config.h>
#include <string.h>

class Sim {
private:
   /* Weather forecasting LUTs */
   Forecast_Lut wind_speed_lut;
   Forecast_Lut wind_dir_lut;
   Forecast_Lut dni_lut;
   Forecast_Lut dhi_lut;

   /* Simulation parameters */
   double control_stop_charge_time;
   int race_start; /* Start time of race in 24 hour */
   int race_end;   /* End time of race in 24 hour */

   /* Internal running state of the simulation */
   Time* curr_time;
   double battery_energy;
   double total_distance;

   /* Route to simulate on and the model to use */
   Car* car;

public:
   /* Loads all LUTs upon construction */
   Sim(Car* model);

   /* Run a full simulation with a car object and a route. Return true if viable 
      @param route: The Route to simulate on
      @param car: The instance of a car to use
      @param speed_profile_kph: The speed to use for each segment
      
      TODO: Add parameters for current time, maximum soc bound and minimum soc bound as a percentage
   */
   bool run_sim(Route route, std::vector<uint32_t> speed_profile_kph);
};

#endif 