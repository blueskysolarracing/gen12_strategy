# Race Simulation 
This framework intends to provide a variety of optimization algorithms and car models in order to identify the best speed profile for the solar car

# Build
From Gen12_Strategy/RaceSim/
1. mkdir build
2. cd build
3. cmake .. (cmake .. -G "MinGW Makefiles" on windows)
4. make install && cd ..
5. ./RaceSim.exe <Path to config>

# To Use
Modify the main.cpp file in order to run a simulation. This would involve creating a car model, a route, a simulation object, and running an optimizer. The following ```main.cpp``` demonstrates the method by which to do this:

```
int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::info);
    ASSERT_EXIT(argc >= 2, "No config file supplied. Exiting");

    /* Load the global configuration file */
    CONFIG_FILE_PATH = argv[1];

    /* Create a model of the car */
    Car* car = Car_Factory::get_car(Config::get_instance()->get_model());
    
    /* Create route */
    Route route = Route();

    /* Create simulator */
    Sim sim = Sim(car);

    /* Create optimizer */
    Optimizer* opt = Opt_Factory::get_optimizer(Config::get_instance()->get_optimizer(), route, sim);
    std::vector<uint32_t> result_speed_profile_km = opt->optimize();
    spdlog::info("Viable Speed Profile: {}", result_speed_profile_km[0]);

    return 0;
}

```

# Scientific Units used 
All units are listed below. Any others not mentioned are also in the base metric system. The configuration file specifies units and is ultimately converted to these units

energy -> kilowatthour\
power -> Watt\
speed -> meters / second\
mass -> kilogram\
temperature -> celsius\
distance -> meters\
angles -> degrees\
force -> newton\
area -> meters squared

When naming variables representing a scientific unit, please include the units if it does not conform to the ones listed above and also is not in the base metric sytem e.g. ```double speed_kph = 70.2;```
