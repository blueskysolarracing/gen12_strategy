# Race Simulation 
This framework intends to provide a variety of optimization algorithms and car models in order to identify the best speed profile for the solar car

# Build
1. mkdir build
2. cd build
3. cmake ..
4. make

# Units
By default, all units are listed below. Any others not mentioned are also in the base metric system.\
energy -> kilowatthour\
power -> Watt\
speed -> meters / second\
mass -> kilogram\
temperature -> celsius\
distance -> meters\
angles -> degrees\
force -> newton\
area -> meters squared\

When naming variables representing a scientific unit, please include the units if it's not in the base metric system or any of the ones listed above e.g. ```double speed_kph = 70.2;```
