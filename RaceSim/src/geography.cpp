#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <units.h>
#include <utilities.h>
#include <geography.h>
#include <cmath>

/* TODO: Check if this function returns in metres */
double get_distance(Coord src_coord, Coord dst_coord) {
	constexpr double R = 6371e3; 

	double phi_1 = src_coord.lat * M_PI/180; 
	double phi_2 = dst_coord.lat * M_PI/180;
	double delPhi = (dst_coord.lat-src_coord.lat) * M_PI/180;
	double delLambda = (dst_coord.lon-src_coord.lon) * M_PI/180;

	double a =  ( sin(delPhi/2) * sin(delPhi/2) ) + ( cos(phi_1) * cos(phi_2) * sin(delLambda/2) * sin(delLambda/2) );
	double c = 2 * atan2(sqrt(a), sqrt(1-a));

	// Haversine distance in km
	double dist_km = (R * c)/1000;

	// calculate altitude
	double alt_1 = src_coord.alt;
	double alt_2 = dst_coord.alt;

	double alt_difference = abs(alt_1-alt_2);

	// Calculate true distance with haversine
	double true_distance = sqrt(dist_km * dist_km + alt_difference * alt_difference) * 1000.0;
	
	return true_distance;
}

/* Gets distance between two lat/lon coordinates */
double get_forecast_coord_distance(ForecastCoord src_coord, ForecastCoord dst_coord) {
	constexpr double R = 6371e3; 

	double phi_1 = src_coord.lat * M_PI/180; 
	double phi_2 = dst_coord.lat * M_PI/180;
	double delPhi = (dst_coord.lat-src_coord.lat) * M_PI/180;
	double delLambda = (dst_coord.lon-src_coord.lon) * M_PI/180;

	double a =  ( sin(delPhi/2) * sin(delPhi/2) ) + ( cos(phi_1) * cos(phi_2) * sin(delLambda/2) * sin(delLambda/2) );
	double c = 2 * atan2(sqrt(a), sqrt(1-a));

	// Haversine distance in km
	double dist_km = (R * c)/1000;

	double alt_difference = 0.0;

	// Calculate true distance with haversine
	double true_distance = sqrt(dist_km * dist_km + alt_difference * alt_difference) * 1000.0;
	
	return true_distance;
}

double get_bearing(Coord src_coord, Coord dst_coord) {
    Coord src_coord_rad = {deg2rad(src_coord.lat), deg2rad(src_coord.lon), src_coord.alt};
    Coord dst_coord_rad = {deg2rad(dst_coord.lat), deg2rad(dst_coord.lon), dst_coord.alt};
    double delta_lon = dst_coord_rad.lon- src_coord_rad.lon;

    double X = cos(dst_coord_rad.lat)*sin(delta_lon);
    double Y = (cos(src_coord_rad.lat)*sin(dst_coord_rad.lat))-(sin(src_coord_rad.lat) * cos(dst_coord_rad.lat)* cos(delta_lon));

    if (delta_lon < 0) {
        return 360 + rad2deg(atan2(X,Y));
    } else {
        return rad2deg(atan2(X,Y));
    }
}
