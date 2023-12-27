#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <units.h>

bool isDouble(std::string str);

/* Calculate haversine distance between two points in metres */
double get_distance(Coord src_coord, Coord dst_coord);

#endif
