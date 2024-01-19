#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>

#define M_PI (3.14159265358979323846264338327950288)
#define DEGREES_IN_PI (180)
#define HOURS_TO_SECONDS (3.6e3)
#define JOULES_TO_KWH (3.6e6)
#define MPS_TO_KPH (3.6)
#define GRAVITY_ACCELERATION (9.81)
const double OVERNIGHT_STEP_SIZE (30.0);

/* The path to the configuration file */
extern std::string CONFIG_FILE_PATH;

/* Car Model to integer mappings */
enum cars
{   
    V1_CAR = 0,
};

/* Optimizer to integer mappings */
enum optimizer 
{
    V1_OPT = 0,
};

#endif