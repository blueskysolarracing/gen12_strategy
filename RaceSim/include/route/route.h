/*
Class for representing the route
*/

#ifndef ROUTE_H
#define ROUTE_H

#include <string.h>
#include <stdlib.h>
#include <units.h>
#include <vector>
#include <unordered_set>

class Route {
protected:
    /* Number of segments to split the route up into */
    uint32_t num_segments;

    /* Starting and Ending indices of all segments */
    std::vector<std::pair<uint32_t, uint32_t>> segments;

    /* Length of each segment */
    std::vector<double> segment_lengths;

    /* Indices of all control stops */
    std::unordered_set<size_t> control_stops;

    /* Points of the route */
    std::vector<Coord> route_points;

    /* Number of points along the route */
    uint32_t num_points;

    /* Total length of the route */
    double route_length;

public:
    /* Initialize the vector of route points */
    Route();

    /* Segment the route into uniform lengths */
    void segment_route_uniform(double length);

    /* Getters */
    uint32_t get_num_segments() const;
    std::vector<std::pair<uint32_t, uint32_t>> get_segments() const;
    std::vector<double> get_segment_lengths() const;
    std::unordered_set<size_t> get_control_stops() const;
    std::vector<Coord> get_route_points() const;
    uint32_t get_num_points() const;

    /* Setters */
    void set_num_segments(uint32_t num);
    void set_segments(std::vector<std::pair<uint32_t, uint32_t>> seg);
    void set_segment_lengths(std::vector<double> seg_lengths);
    void set_control_stops(std::unordered_set<size_t> c_stops);
    void set_route_points(std::vector<Coord> new_route_points);
};

#endif
