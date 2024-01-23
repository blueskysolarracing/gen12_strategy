# Race Simulation 
This framework intends to provide a variety of optimization algorithms and car models in order to identify the best speed profile for the solar car

# Build
From Gen12_Strategy/RaceSim/
1. mkdir build
2. cd build
3. cmake .. (cmake .. -G "MinGW Makefiles" if on windows)
4. make
5. ./racesim.exe

# To Use
As of January 12th, 2024, this framework can only assess the viability of a speed profile given the gen 11.5 car model and the necessary LUTs. The following ```main.cpp``` demonstrates the method by which to do this:

```
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

    /* Create optimizer (constant speed) */
    Optimizer* opt = Opt_Factory::get_optimizer(Config::get_instance()->get_opt_type(), route, sim);

    /* Optimize */
    std::vector<uint32_t> result_speed_profile_kph = opt->optimize();
    std::cout << "Viable Speed Profile: " << result_speed_profile_kph[0] << std::endl;

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
