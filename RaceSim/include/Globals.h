#ifndef GLOBALS_H
#define GLOBALS_H
#include <string>
#include "spdlog/spdlog.h"

#define PI (3.14159265358979323846264338327950288)
#define DEGREES_IN_PI (180)
#define HOURS_TO_SECONDS (3.6e3)
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

/* Optimizer to integer mappings */
enum optimizer 
{
    V1_OPT = 0,
};

/* Custom no-exit assertion */
#define ASSERT(expr, message) \
    do { \
        if (!(expr)) { \
            spdlog::error("Assertion failed: {}", message); \
            return; \
        } \
    } while(0)

/* Custom exit assertion */
#define ASSERT_EXIT(expr, message) \
    do { \
        if (!(expr)) { \
            spdlog::error("Assertion failed: {}", message); \
            exit(0); \
        } \
    } while(0)

#endif