#include "spdlog/spdlog.h"
#include <v1_opt.h>
#include <base_opt.h>
#include <vector>
#include <config.h>
#include <route.h>
#include <sim.h>
#include <pthread.h>
#include <filesystem>

std::vector<uint32_t> V1_Opt::optimize() {
    /* Loop from speeds 1 -> max. speed to get the maximum viable speed */
    std::vector<uint32_t> speed_profile_kph(1);
    bool last_speed_viability = false;
    std::vector<uint32_t> last_speed_profile_kph = speed_profile_kph;
    int max_speed = Config::get_instance()->get_max_speed();
    int min_speed = Config::get_instance()->get_min_speed();

    bool save_csv = Config::get_instance()->get_save_csv();

    for (int i=min_speed; i<=max_speed; i++) {
        speed_profile_kph[0] = i;

        if (save_csv) {
            std::filesystem::create_directory("Results/");
        }

        /* Run the simulation */
        bool current_speed_viability = sim.run_sim(route, speed_profile_kph);

        /* Log the simulation */
        if (save_csv) {
            sim.write_result("Results/" + std::to_string(speed_profile_kph[0]) + ".csv");
        }

        if (current_speed_viability) {
            spdlog::info(std::to_string(i) + " kph is viable.");
        } else {
            spdlog::info(std::to_string(i) + " kph is not viable.");
        }
        if (last_speed_viability && !current_speed_viability) {
            return last_speed_profile_kph;
        }
        last_speed_viability = current_speed_viability;
        last_speed_profile_kph = speed_profile_kph;
    }

    if (!last_speed_viability) {
        speed_profile_kph[0] = 0;
    }
    return speed_profile_kph;
}

V1_Opt::V1_Opt(Sim sim, Route route) : Optimizer(sim, route) {}
