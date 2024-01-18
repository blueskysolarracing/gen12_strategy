#include <base_opt.h>
#include <sim.h>
#include <route.h>

Optimizer::Optimizer(Sim simulator, Route path) :
    sim(simulator),
    route(path)
{}