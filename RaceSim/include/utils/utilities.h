/* A variety of general purpose utility functions */

#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <units.h>
#include <set>

/* Determine if a string can be represented by a double */
bool isDouble(std::string str);

/** Convert a comma seperate string of doubles into a set 
 * @param input comma seperated string of integers
 * example: convert_string_to_set("2, 3, 4, 5") --> std::set({1,2,3,4,5})
*/
std::set<size_t> convert_string_to_int_set(std::string input);

/** Convert a local time string to a time_t unix epoch time 
 * @param input Local time represented in YYYY-MM-DD HH:MM:SS format
*/
time_t create_time(std::string input);

/** Convert a comma seperated string into a coordinate 
 * @param input: comma seperated string of doubles in <lat>, <lon>, <alt> format
 * example: create_coord("2.0, 3.0, 4.9") --> Coord({lat: 2.0, lon: 3.0, alt: 4.9})
*/
Coord create_coord(std::string input);

#endif
