#include <v1_opt.h>
#include <base_opt.h>
#include <vector>
#include <config.h>
#include <route.h>
#include <sim.h>
#include <pthread.h>

std::vector<uint32_t> V1_Opt::optimize() {
    /* Loop from speeds 1 -> max. speed to get the maximum viable speed */
    std::vector<uint32_t> speed_profile_kph(1);
    bool last_speed_viability = false;
    int max_speed = Config::get_instance()->get_max_speed();

    for (int i=1; i<=max_speed; i++) {
        speed_profile_kph[0] = i;
        bool current_speed_viability = sim.run_sim(route, speed_profile_kph);

        if (current_speed_viability) {
            std::cout << i << " kph is viable." << std::endl;
        } else {
            std::cout << i << " kph is not viable." << std::endl;
        }
        if (last_speed_viability && !current_speed_viability) {
            return speed_profile_kph;
        }
    }

    speed_profile_kph[0] = 0;
    return speed_profile_kph;
}

V1_Opt::V1_Opt(Sim sim, Route route) : Optimizer(sim, route) {}
