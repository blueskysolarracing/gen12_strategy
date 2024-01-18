/* 
Base class for optimization algorithms
*/

#ifndef BASE_OPT_H
#define BASE_OPT_H

#include <base_car.h>
#include <route.h>
#include <sim.h>

class Optimizer {
protected: 
    /* Route to optimize on */
    Sim sim;
    Route route;

    /* Output speed profile */
    std::vector<double> speed_profile;
public:

    Optimizer(Sim simulator, Route path);

    /* Apply the optimization algorithm, output a speed profile over
       the segments of the race
    */
    virtual std::vector<uint32_t> optimize() = 0;
};

#endif
