#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <units.h>
#include <utilities.h>
#include <geography.h>
#include <custom_time.h>
#include <cmath>

/* TODO: Check if this function returns in metres */
double get_distance(Coord src_coord, Coord dst_coord) {
	constexpr double R = 6371e3; 

	double phi_1 = src_coord.lat * PI/180; 
	double phi_2 = dst_coord.lat * PI/180;
	double delPhi = (dst_coord.lat-src_coord.lat) * PI/180;
	double delLambda = (dst_coord.lon-src_coord.lon) * PI/180;

	double a =  ( sin(delPhi/2) * sin(delPhi/2) ) + ( cos(phi_1) * cos(phi_2) * sin(delLambda/2) * sin(delLambda/2) );
	double c = 2 * atan2(sqrt(a), sqrt(1-a));

	// Haversine distance in m
	double dist_km = (R * c);

	// calculate altitude difference
	double alt_1 = src_coord.alt;
	double alt_2 = dst_coord.alt;

	double alt_difference = abs(alt_1-alt_2);

	// Calculate true distance with haversine
	double true_distance = sqrt(dist_km * dist_km + alt_difference * alt_difference);
	
	return true_distance;
}

/* Gets distance between two lat/lon coordinates */
double get_forecast_coord_distance(ForecastCoord src_coord, ForecastCoord dst_coord) {
	constexpr double R = 6371e3; 

	double phi_1 = src_coord.lat * PI/180; 
	double phi_2 = dst_coord.lat * PI/180;
	double delPhi = (dst_coord.lat-src_coord.lat) * PI/180;
	double delLambda = (dst_coord.lon-src_coord.lon) * PI/180;

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

SolarAngle get_az_el_from_bearing(double bearing, Coord coord, Time time) {
    SolarAngle sun = SolarAngle();
    get_az_el(time.get_utc_time_point(), coord.lat, coord.lon, meters2km(coord.alt), &sun.Az, &sun.El);
    // Get the relative azimuth angle based on bearing from true north. 
    // e.g. if the car was pointing south (180 degree bearing) and the azimuth of the sun was 90 degrees (east)
    // then the relative azimuth is 270 degrees
    sun.Az = sun.Az + 180 - bearing;
    if (sun.Az < 0) {
        sun.Az = 360 + sun.Az;
    } else if (sun.Az > 360) {
        sun.Az = sun.Az - 360;
    }
    return sun;
}

double get_speed_relative_to_wind(double car_speed, double car_bearing, Wind wind) {
    // Subtract wind speed because the bearing points the wind towards and the car points from the origin.

    // Make wind bearing point away from the origin
    double wind_bearing = wind.bearing < 180.0 ? wind.bearing + 180.0 : wind.bearing - 180.0;

    // Calculate angle between wind and car vector
    double interior_angle = std::abs(wind_bearing - car_bearing);	
    double relative_wind_speed = 0.0;

    if (interior_angle < 90.0 && interior_angle > 0.0) { // tailwind
        relative_wind_speed = -1 * cos(deg2rad(interior_angle)) * wind.speed;
    } else if (interior_angle > 90.0 && interior_angle < 180.0) { // headwind
        relative_wind_speed = sin(deg2rad(interior_angle-90.0)) * wind.speed;
    } else if (interior_angle > 180.0 && interior_angle < 270.0) { // headwind
        relative_wind_speed = cos(deg2rad(interior_angle-180.0)) * wind.speed;
    } else if (interior_angle > 270.0 && interior_angle < 360.0) { // tailwind
        relative_wind_speed = -1 * sin(deg2rad(interior_angle - 270.0)) * wind.speed;
    } else if (interior_angle == 180.0) { // direct headwind
        relative_wind_speed = wind.speed;
    } else if (interior_angle == 0.0) { // direct tailwind
        relative_wind_speed = -1 * wind.speed;
    }

    return car_speed + relative_wind_speed;
}

double julian_day(time_t utc_time_point) {

    // Extract UTC Time
    struct tm* tm = localtime(&utc_time_point);
    double year = tm->tm_year + 1900;
    double month = tm->tm_mon + 1;
    double day = tm->tm_mday;
    double hour = tm->tm_hour;
    double min = tm->tm_min;
    double sec = tm->tm_sec;

    if (month <= 2) {
        year -= 1;
        month += 12;
    }

    double jd = floor(365.25*(year + 4716.0)) + floor(30.6001*(month + 1.0)) + 2.0 -
        floor(year / 100.0) + floor(floor(year / 100.0) / 4.0) + day - 1524.5 +
        (hour + min / 60 + sec / 3600) / 24;

    return jd;
}

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

// Programed by Darin C.Koblick 2 / 17 / 2009
//
//              Darin C.Koblick 4 / 16 / 2013 Vectorized for Speed
//                                         Allow for MATLAB Datevec input in
//                                         addition to a UTC string.
//                                         Cleaned up comments and code to
//                                         avoid warnings in MATLAB editor.
//
//				Kevin Godden 9/1/2020      Ported from Matlab to C++, tried to change as little as possible.
//                                         this is a non-vectorised port.
//
//--------------------------------------------------------------------------
//
// External Function Call Sequence :
//
// double lat = 52.975;
// double lon = -6.0494;
// double altitude = 0;
//
// double Az = 0.0;
// double El = 0.0;
// SolarAzEl(time(NULL), lat, lon, 0, &Az, &El);
// 
// printf("Azimuth: %f\n", Az);
// printf("Elevation: %f\n", El);
//
// Or to calculate Az & El for an arbitary UTC time:
//
//
// tm utc;
// tm_year is time since 1900
// utc.tm_year = y - 1900;
// Month is zero based, i.e. Jan is month 0
// utc.tm_mon = m - 1;
// utc.tm_mday = d;
// utc.tm_hour = 10;
// utc.tm_min = 16;
// utc.tm_sec = 00;
// utc.tm_isdst = 0;
// 
// Get UTC time_t val
// tim = timegm(&utc);	// or _mkgmtime() on windows
// 
// double altitude = 0;
// double Az = 0.0;
// double El = 0.0;
// 
// double lat = 52.975;
// double lon = -6.0494;
// 
// SolarAzEl(tim, lat, lon, 0, &Az, &El);
// 
// printf("Az: %f\n", Az);
// printf("El: %f\n", El);
// 
//
// Function Description :
//
// SolarAzEl will ingest a Universal Time, and specific site location on earth
// it will then output the solar Azimuth and Elevation angles relative to that
// site.
//
// Input Description :
//
// utc_time_point : time_t containing target time for sun position calculations.
//
// Lat : Site Latitude in degrees -90:90->S(-) N(+)
//
// Lon : Site Longitude in degrees -180:180 W(-) E(+)
//
// Alt : Altitude of the site above sea level(Km)
//
// Output Description :
//  Az    Azimuth location of the sun(deg)
//  El    Elevation location of the sun(deg)
//
//
// Source References :
// Solar Position obtained from :
// http ://stjarnhimlen.se/comp/tutorial.html#5
//

void get_az_el(time_t utc_time_point, double Lat, double Lon, double Alt, double* Az, double* El) {
	double jd = julian_day(utc_time_point);

	double d = jd - 2451543.5;
	
	// Keplerian Elements for the Sun(geocentric)
	double w = 282.9404 + 4.70935e-5*d; // (longitude of perihelion degrees)
	// a = 1.000000; % (mean distance, a.u.)
	double e = 0.016709 - 1.151e-9*d; // (eccentricity)
	double M = fmod(356.0470 + 0.9856002585*d, 360.0); // (mean anomaly degrees)
		
	double L = w + M; // (Sun's mean longitude degrees)

	double oblecl = 23.4393 - 3.563e-7*d; // (Sun's obliquity of the ecliptic)

	// auxiliary angle
	double  E = M + (180 / PI)*e*sin(M*(PI / 180))*(1 + e*cos(M*(PI / 180)));

	// rectangular coordinates in the plane of the ecliptic(x axis toward perhilion)
	double x = cos(E*(PI / 180)) - e;
	double y = sin(E*(PI / 180))*sqrt(1 - pow(e, 2));

	// find the distance and true anomaly
	double r = sqrt(pow(x,2) + pow(y,2));
	double v = atan2(y, x)*(180 / PI);

	// find the longitude of the sun
	double lon = v + w;

	// compute the ecliptic rectangular coordinates
	double xeclip = r*cos(lon*(PI / 180));
	double yeclip = r*sin(lon*(PI / 180));
	double zeclip = 0.0;
	//rotate these coordinates to equitorial rectangular coordinates
	double xequat = xeclip;

	double yequat = yeclip*cos(oblecl*(PI / 180)) + zeclip * sin(oblecl*(PI / 180));

	double zequat = yeclip*sin(23.4406*(PI / 180)) + zeclip * cos(oblecl*(PI / 180));
	// convert equatorial rectangular coordinates to RA and Decl:
	r = sqrt(pow(xequat, 2) + pow(yequat, 2) + pow(zequat, 2)) - (Alt / 149598000); //roll up the altitude correction
	double RA = atan2(yequat, xequat)*(180 / PI);

	double delta = asin(zequat / r)*(180 / PI);
	
	// Following the RA DEC to Az Alt conversion sequence explained here :
	// http ://www.stargazing.net/kepler/altaz.html
	//	Find the J2000 value
	//	J2000 = jd - 2451545.0;
	//hourvec = datevec(UTC);
	//UTH = hourvec(:, 4) + hourvec(:, 5) / 60 + hourvec(:, 6) / 3600;

	// Get UTC representation of time / C++ Specific
	tm *ptm;
	ptm = gmtime(&utc_time_point);
	double UTH = (double)ptm->tm_hour + (double)ptm->tm_min / 60 + (double)ptm->tm_sec / 3600;

	// Calculate local siderial time
	double GMST0 = fmod(L + 180, 360.0) / 15;

	double SIDTIME = GMST0 + UTH + Lon / 15;
	
	// Replace RA with hour angle HA
	double HA = (SIDTIME*15 - RA);

	// convert to rectangular coordinate system
	x = cos(HA*(PI / 180))*cos(delta*(PI / 180));

	y = sin(HA*(PI / 180))*cos(delta*(PI / 180));
	double z = sin(delta*(PI / 180));

	// rotate this along an axis going east - west.
	double xhor = x*cos((90 - Lat)*(PI / 180)) - z*sin((90 - Lat)*(PI / 180));

	double yhor = y;
	double zhor = x*sin((90 - Lat)*(PI / 180)) + z*cos((90 - Lat)*(PI / 180));
	
	// Find the h and AZ
	*Az = atan2(yhor, xhor)*(180 / PI) + 180;
	*El = asin(zhor)*(180 / PI);
}