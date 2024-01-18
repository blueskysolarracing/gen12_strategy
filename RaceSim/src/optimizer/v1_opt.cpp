#include <v1_opt.h>
#include <base_opt.h>
#include <vector>
#include <config.h>
#include <route.h>
#include <sim.h>

std::vector<uint32_t> V1_Opt::optimize() {
    /* Loop from speeds 1 -> max. speed to get the maximum viable speed */
    std::vector<uint32_t> speed_profile(1);
    bool last_speed_viability = false;
    int max_speed = Config::get_instance()->get_max_speed();

    for (int i=1; i<=max_speed; i++) {
        speed_profile[0] = i;
        bool current_speed_viability = sim.run_sim(route, speed_profile);
        if (last_speed_viability && !current_speed_viability) {
            return speed_profile;
        }
    }

    speed_profile[0] = 0;
    return speed_profile;
}

V1_Opt::V1_Opt(Sim sim, Route route) : Optimizer(sim, route) {}
