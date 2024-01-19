/* Statically generate different car models */

#include <car_factory.h>
#include <base_car.h>
#include <Globals.h>
#include <v1_car.h>

Car* Car_Factory::get_car(int car_type) {
    if (car_type == V1_CAR) {
        std::cout << "---------------------Using V1 Car Model----------------------" << std::endl;
        return (Car*) new V1_Car();
    } else {
        return nullptr;
    }
}