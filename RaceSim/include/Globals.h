#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>
#include "spdlog/spdlog.h"

#define PI (3.14159265358979323846264338327950288)
#define DEGREES_IN_PI (180)
#define HOURS_TO_SECONDS (3.6e3)
#define MINUTES_TO_SECONDS 60.0
#define HOURS_TO_MINUTES 60.0
#define JOULES_TO_KWH (3.6e6)
#define MPS_TO_KPH (3.6)
#define GRAVITY_ACCELERATION (9.81)
#define KM_TO_M (1000.0)
const double OVERNIGHT_STEP_SIZE (30.0);

/* The path to the configuration file */
extern std::string CONFIG_FILE_PATH;

/* Car Model to integer mappings */
enum cars
{   
    V1_CAR = 0,
};
extern std::unordered_map<std::string, cars> config_to_car_model;
extern std::string DEFAULT_CAR;

/* Optimizer to integer mappings */
enum optimizer 
{
    CONSTANT = 0,
};
extern std::unordered_map<std::string, optimizer> config_to_optimizer;
extern std::string DEFAULT_OPTIMIZER;

#endif