/* Generate optimizers */

#ifndef OPT_FACTORY_H
#define OPT_FACTORY_H

#include <base_opt.h>
#include <route.h>
#include <sim.h>

class Opt_Factory {
public:
    static Optimizer* get_optimizer(int opt_type, Route route, Sim simulator);
};

#endif