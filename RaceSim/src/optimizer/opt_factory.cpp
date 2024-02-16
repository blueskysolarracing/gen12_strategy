#include "spdlog/spdlog.h"
#include <opt_factory.h>
#include <v1_opt.h>
#include <base_opt.h>
#include <Globals.h>

std::unordered_map<std::string, optimizer> config_to_optimizer = {
    {"Constant", optimizer::CONSTANT},
};
std::string DEFAULT_OPTIMIZER = "Constant";

Optimizer* Opt_Factory::get_optimizer(std::string opt_type, Route route, Sim simulator) {
    optimizer opt;
    if (config_to_optimizer.find(opt_type) != config_to_optimizer.end()) {
        opt = config_to_optimizer[opt_type];
    } else {
        opt = config_to_optimizer[DEFAULT_OPTIMIZER]; /* Default to constant speed */
    }

    if (opt == optimizer::CONSTANT) {
        spdlog::info("Using constant speed optimizer.");
        return (Optimizer*) new V1_Opt(simulator, route);
    } else {
        return nullptr;
    }
}