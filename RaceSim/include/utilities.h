/* A variety of general purpose utility functions and data types */

#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <units.h>
#include <set>

/* Determine if a string can be represented by a double */
bool isDouble(std::string str);

/* Convert a comma seperated string of numbers into an integer set */
std::set<size_t> create_control_stops_set(std::string input);

// Return a time_t (unix epoch time) from a local time string in YYYY-MM-DD HH:MM:SS format
time_t create_time(std::string input);

// Create a coordinate struct from a lat, lon, altitude string
Coord create_coord(std::string input);

#endif
