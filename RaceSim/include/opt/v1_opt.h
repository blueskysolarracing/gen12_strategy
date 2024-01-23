/*
Optimizer that seeks a single constant speed along the route
*/

#ifndef V1_OPT_H
#define V1_OPT_H

#include <base_opt.h>
#include <route.h>
#include <sim.h>

class V1_Opt : Optimizer {
public:
    V1_Opt(Sim sim, Route route);
    std::vector<uint32_t> optimize() override;
};

#endif