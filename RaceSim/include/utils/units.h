/*
Definitions and functions for scientific units, custom objects, conversion functions and utility functions
*/

#ifndef UNITS_H
#define UNITS_H

#include <Globals.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>
#include <chrono>
#include <date.h>

/* Custom units */

/* A forecast coordinate is represented only by latitude and longitude */
struct ForecastCoord {
	double lat;
	double lon;

	ForecastCoord(double latitude, double longitude) : lat(latitude), lon(longitude) {}
	ForecastCoord() : lat(0), lon(0) {}
};

struct Coord {
	double lat;
	double lon;
	double alt;

	Coord(double latitude, double longitude, double altitude) : lat(latitude), lon(longitude), alt(altitude) {}
	Coord() : lat(0), lon(0), alt(0) {}

	/* Conversion operator from type ForecastCoord to Coord by clearing the altitude value */
	Coord(const struct ForecastCoord& fc) : lat(fc.lat), lon(fc.lon), alt(0.0) {}
};

struct Wind {
	double bearing;
	double speed;

	Wind(double b, double s) : bearing(b), speed(s) {}
	Wind() : bearing(0), speed(0) {}
};

struct SolarAngle {
	double Az;
	double El;

	SolarAngle(double az_angle, double el_angle) : Az(az_angle), El(el_angle) {}
	SolarAngle() : Az(0.0), El(0.0) {}
};

struct TireCoeff {
	double yint;
	double slope;

	TireCoeff(double y, double s) : yint(y), slope(s) {}
	TireCoeff() : yint(0), slope(0) {}
};

struct Irradiance {
	double dni;
	double dhi;

	Irradiance(double _dni, double _dhi) : dni(_dni), dhi(_dhi) {}
	Irradiance() : dni(0), dhi(0) {}
};

/* Each energy loss/gain (e.g. rolling resistance, aerodynamic, gravity) is characterized by both its
   actual energy lost/gained and the instataneous power drawn/generated
*/
struct Energy_Change {
	double power;
	double energy;

	Energy_Change(double p, double e) : power(p), energy(e) {}
	Energy_Change() : power(0), energy(0) {}
};

/* Wraps the c++ time data structures 
   Note: We use the date and chrono libraries in order to make conversions between tm and time_t representations since 
   mktime in c++ implicitly does a timezone conversion based on the user's machine.
*/
class Time {
public:
	/* Use the current time as the starting point */
    Time();

	/* Specify a starting time */
	Time(time_t local_time_point, double utc_adjustment);

	/* Local 24 hour time */
    inline int get_local_hours() { return m_datetime_local.tm_hour; };

	/* Unix epoch UTC timestamp */
	inline time_t get_utc_time_point() { return t_datetime_utc; }

	/* Unix epoch local timestamp */
	inline time_t get_local_time_point() { return t_datetime_local; }

	/* tm struct UTC timestamp */
	inline tm get_utc_tm() { return m_datetime_utc; }

	/* tm struct local timestamp */
	inline tm get_local_tm() { return m_datetime_local; }

	/* Formats time according to the string found in forecast csv's */
	uint64_t get_forecast_csv_time();

	/* Advances the timestamp by a certain amount of seconds */
    void update_time_seconds(double seconds);

	/* Manually update the time structs */
	void update_current_time(tm curr_time_local, tm curr_time_utc) {m_datetime_local = curr_time_local; m_datetime_utc = curr_time_utc;} 

	/* Print human readable local time */
	void print_local_readable_time() {std::cout << get_local_readable_time();}

	/* Print human readable utc time */
	void print_utc_readable_time() {std::cout << get_utc_readable_time() << std::endl;}
	
	/* Get human readable local time */
	inline std::string get_local_readable_time() {return asctime(&m_datetime_local);};

	/* Get human readable utc time */
	inline std::string get_utc_readable_time() {return asctime(&m_datetime_utc);}
private:
	/* C++ tm structs and unix epoch times for both the local time and utc timepoint */
    tm m_datetime_local;
	tm m_datetime_utc;

	time_t t_datetime_local;
	time_t t_datetime_utc;

	/* Special field for milliseconds since the tm struct's resolution is up to seconds */
    double m_milliseconds;
};

/* Unit conversions */
inline double rad2deg(double radians) { return radians * DEGREES_IN_PI / M_PI; }
inline double deg2rad(double degrees) { return degrees * M_PI / DEGREES_IN_PI; }
inline double hours2secs(double hours) { return hours * HOURS_TO_SECONDS; }
inline double secs2hours(double seconds) { return seconds / HOURS_TO_SECONDS; }
inline double kph2mps(double kph) { return kph / MPS_TO_KPH; }
inline double mps2kph(double mps) { return mps * MPS_TO_KPH; }
inline double joules2kwh(double joules) { return joules / JOULES_TO_KWH; }
inline double watts2kwh(double time, double watts) { return watts * (secs2hours(time) / 1000.0);}
inline double meters2km(double m) {return m/KM_TO_M;}
inline double km2meters(double km) {return km * KM_TO_M;}

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