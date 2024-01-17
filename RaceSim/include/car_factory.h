/* Statically generate different car models */

#ifndef CAR_FACTORY_H
#define CAR_FACTORY_H

#include <base_car.h>
#include <v1_car.h>

class Car_Factory {
public:
    static Car* get_car(int car_int);
};

#endif
