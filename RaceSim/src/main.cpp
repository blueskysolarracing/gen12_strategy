/* Starting point of a race simulation */

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

std::string CONFIG_FILE_PATH;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Please provide a config file." << std::endl;
        return 0;
    }

    /* Load the global configuration file */
    CONFIG_FILE_PATH = argv[1];

    /* Create a model of the car */
    Car* car = Car_Factory::get_car(Config::get_instance()->get_car_type());
    
    /* Create route */
    Route route = Route();

    /* Create simulator */
    Sim sim = Sim(car);

    return 0;
}