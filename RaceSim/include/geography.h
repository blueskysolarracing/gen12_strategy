/* Functions for geographical related calculations */

#ifndef GEOGRAPHY_H
#define GEOGRAPHY_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <units.h>

/* Calculate haversine distance between two points */
double get_distance(Coord src_coord, Coord dst_coord);

/* Bearing in degrees clockwise from true north */
double get_bearing(Coord src_coord, Coord dst_coord);

#endif
