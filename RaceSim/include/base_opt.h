/* 
Base class for optimization algorithms
*/

#ifndef BASE_OPT_H
#define BASE_OPT_H


#include <base_car.h>
#include <route.h>

class Base_Opt {
protected: 
    /* Output speed profile */
    std::vector<double> speed_profile;
public:
    /* Apply the optimization algorithm */
    virtual void optimize() = 0;
};

#endif
