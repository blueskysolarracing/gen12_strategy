#include "spdlog/spdlog.h"
#include <opt_factory.h>
#include <v1_opt.h>
#include <base_opt.h>
#include <Globals.h>

Optimizer* Opt_Factory::get_optimizer(std::string opt_type, Route route, Sim simulator) {
    if (opt_type == "Constant") {
        spdlog::info("Using constant speed optimizer.");
        return (Optimizer*) new V1_Opt(simulator, route);
    } else {
        return nullptr;
    }
}