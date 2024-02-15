/* Functions for geographical related calculations */

#ifndef GEOGRAPHY_H
#define GEOGRAPHY_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <units.h>
#include <custom_time.h>

/* Calculate haversine distance between two lat/lon/alt points */
double get_distance(Coord src_coord, Coord dst_coord);

/* Calculate distance between two lat/lon points */
double get_forecast_coord_distance(ForecastCoord src_coord, ForecastCoord dst_coord);

/* Bearing in degrees clockwise from true north */
double get_bearing(Coord src_coord, Coord dst_coord);

/* Azimuth and elevation of the sun from the car's location, bearing and time of day
   TODO: Need to make sure that this is degrees clockwise from the nose of the car!!!
*/
SolarAngle get_az_el_from_bearing(double bearing, Coord coord, Time time);

/* Get car speed relative to wind - only headwind considered */
double get_speed_relative_to_wind(double car_speed, double car_bearing, Wind wind);

/* Get julian day (days since first julian period) from a utc time */
double julian_day(time_t utc_time_point);

/*
Copyright(c) 2010, Darin Koblick
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met :

*Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
void get_az_el(time_t utc_time_point, double Lat, double Lon, double Alt, double* Az, double* El); 
#endif
