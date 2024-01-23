/* Functions for geographical related calculations */

#ifndef GEOGRAPHY_H
#define GEOGRAPHY_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <units.h>

/* Calculate haversine distance between two lat/lon/alt points */
double get_distance(Coord src_coord, Coord dst_coord);

/* Calculate distance between two lat/lon points */
double get_forecast_coord_distance(ForecastCoord src_coord, ForecastCoord dst_coord);

/* Bearing in degrees clockwise from true north */
double get_bearing(Coord src_coord, Coord dst_coord);

#endif
