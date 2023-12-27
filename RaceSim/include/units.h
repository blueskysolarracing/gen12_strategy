#ifndef UNITS_H
#define UNITS_H

#include <Globals.h>
#include <vector>
#include <string>
#include <ctime>
#include <iostream>

/* Custom units */

struct Coord {
	double lat;
	double lon;
	double altitude;

	Coord(double latitude, double longitude, double altitude) : lat(latitude), lon(longitude), altitude(altitude) {}
	Coord() : lat(0), lon(0), altitude(0) {}
};

struct Wind {
	double bearing_degrees;
	double speed_kph;

	Wind(double b_degrees, double s_kph) : bearing_degrees(b_degrees), speed_kph(s_kph) {}
};

struct SolarAngle {
	double Az;
	double El;
};

/* Represents an efficicency lookup table loaded in from a csv */
struct EfficiencyLut {
	std::vector<double> column_values;
	std::vector<double> row_values;
	std::vector<std::vector<double>> values;

	void load_eff_lut(std::string lut_path);
};

/* Wraps the C++ epoch time to represent a timestamp */
class Time {
public:
    Time();
    inline int get_hours() { return m_datetime_local.tm_hour; };
	inline time_t get_utc_time_point() { return mktime(&m_datetime_utc); } // Returns time in unix epoch time
	uint64_t get_forecast_csv_time();
    void update_time_seconds(double seconds);
	void update_current_time(tm curr_time_local, tm curr_time_utc) {m_datetime_local = curr_time_local; m_datetime_utc = curr_time_utc;}
	void print_readable_time() {std::cout << m_datetime_local.tm_year + 1900 << "-" << m_datetime_local.tm_mon+1 << "-" << m_datetime_local.tm_mday << "-" << m_datetime_local.tm_hour << "-" << m_datetime_local.tm_min << "-" << m_datetime_local.tm_sec <<std::endl;}
	std::string get_readable_time();
private:
    tm m_datetime_local;
	tm m_datetime_utc;
    double m_milliseconds;
};

/* Unit conversions */
inline double rad2deg(double radians) { return radians * 180 / M_PI; }
inline double deg2rad(double degrees) { return degrees * M_PI / 180; }
inline double hours2secs(double hours) { return hours * 3.6e3; }
inline double secs2hours(double seconds) { return seconds / 3.6e3; }
inline double kph2mps(double kph) { return kph / 3.6; }
inline double joules2kwh(double joules) { return joules / 3.6e6; }

/* Get bearing in degrees clockwise from true north */
double get_bearing(Coord src_coord, Coord dst_coord);

/* Get azimuth and elevation of the sun from the car's location, bearing and time of day */
SolarAngle get_az_el_from_bearing(double bearing_degrees, Coord coord, Time time);

/* Get car speed relative to wind - only headwind considered */
double get_speed_relative_to_wind_kph(double car_speed_kph, double car_bearing_degrees, Wind wind);

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