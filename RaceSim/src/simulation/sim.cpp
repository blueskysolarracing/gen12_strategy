#include <sim.h>
#include <stdlib.h>
#include <assert.h>
#include <units.h>
#include <unordered_set>

bool run_sim(Route route, Base_Car* car, std::vector<uint32_t> speed_profile) {
    assert(speed_profile.size() == route.get_num_segments());

    uint32_t num_points = route.get_num_points();
    std::vector<Coord> route_points = route.get_route_points();
    std::vector<std::pair<uint32_t, uint32_t>> segments = route.get_segments();
    std::unordered_set<uint32_t> control_stops = route.get_control_stops();

    assert(segments.size() == speed_profile.size());

    double net_energy = car->get_max_soc();

    uint32_t segment_counter = 0;
    std::pair<uint32_t, uint32_t> current_segment = segments[segment_counter];
    uint32_t current_speed = speed_profile[segment_counter];

    /* Todo, make this start at some set time */
    Time curr_time = Time();

    for (size_t idx=0; idx<num_points-1; idx++) {
        Coord coord_one = route_points[idx];
        Coord coord_two = route_points[idx+1];
        double energy_change = 0.0;

        /* Control Stop */
        if (control_stops.find(idx) != control_stops.end()) {
            //energy_change += car->compute_static_energy(coord_one, curr_time);
        }

        /* Move from point one to point two */
        if (idx > current_segment.second) {
            segment_counter++;
            current_segment = segments[segment_counter];
            current_speed = speed_profile[segment_counter];
        }

        energy_change += car->compute_travel_energy(coord_one, coord_two, current_speed);

        net_energy += energy_change;

        /* Penalize a minimum soc bound TODO */
        if (net_energy < 0.0) {
            return false;
        }
    }

    return true;
}