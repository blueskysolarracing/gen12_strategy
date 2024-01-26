/* Starting point of a race simulation */

#include "spdlog/spdlog.h"
#include <stdlib.h>
#include <sim.h>
#include <config.h>
#include <string.h>
#include <Globals.h>
#include <stdio.h>
#include <car_factory.h>
#include <base_car.h>
#include <route.h>
#include <sim.h>
#include <base_opt.h>
#include <opt_factory.h>

std::string CONFIG_FILE_PATH;

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::info);
    ASSERT_EXIT(argc >= 2, "No config file supplied. Exiting");

    /* Load the global configuration file */
    CONFIG_FILE_PATH = argv[1];

    /* Create a model of the car */
    Car* car = Car_Factory::get_car(Config::get_instance()->get_car_type());
    
    /* Create route */
    Route route = Route();

    /* Create simulator */
    Sim sim = Sim(car);

    /* Create optimizer */
    Optimizer* opt = Opt_Factory::get_optimizer(Config::get_instance()->get_opt_type(), route, sim);
    std::vector<uint32_t> result_speed_profile_km = opt->optimize();
    spdlog::info("Viable Speed Profile: {}", result_speed_profile_km[0]);

    return 0;
}
