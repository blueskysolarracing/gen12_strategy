#include <stdio.h>
#include <utilities.h>
#include <assert.h>
#include <cassert>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <Globals.h>

bool isDouble(std::string str) {
	if (str[0] == '-' && str.size() >= 2) {
		return isdigit(str[1]);
	}
	return isdigit(str[0]);
}

std::set<size_t> create_control_stops_set(std::string input) {
	std::set<size_t> result_set;

    std::stringstream ss(input);
    std::string value;

    while (std::getline(ss, value, ',')) {
        // Convert substring into integer and insert into set
        result_set.insert(std::stoi(value));
    }

    return result_set;
}

tm create_time(std::string input, double utc_adjustment) {
    const int NUM_TIME_ARGS = 6;
    const int YEAR = 0;
    const int MONTH = 1;
    const int DAY = 2;
    const int HOUR = 3;
    const int MINUTE = 4;
    const int SECOND = 5;

    std::stringstream ss(input);

    std::string value;
    std::tm current_time;

    std::vector<int> time_arguments(NUM_TIME_ARGS);

    for (int i=0; i<NUM_TIME_ARGS; i++) {
        std::getline(ss, value, '/');

        time_arguments[i] = std::stoi(value);
    }

    current_time.tm_year = time_arguments[YEAR] - 1900;
    current_time.tm_mon = time_arguments[MONTH] - 1;
    current_time.tm_mday = time_arguments[DAY];
    current_time.tm_hour = time_arguments[HOUR];
    current_time.tm_min = time_arguments[MINUTE];
    current_time.tm_sec = time_arguments[SECOND];
    current_time.tm_isdst = -1; // not daylight savings time

    current_time.tm_wday = 0; // weekday arbitrarily set to 0
    current_time.tm_yday = 0; 

    return current_time;
}

// Create a coordinate struct from a lat, lon, altitude string
Coord create_coord(std::string input) {
    double lat, lon, alt;
    lat = lon = alt = -1.0;

    std::stringstream ss(input);
    std::string value;

    int count = 1;
    while (std::getline(ss, value, ',')) {
        if (count == 1) {
            lat = std::stod(value); 
        } else if (count == 2) {
            lon = std::stod(value);
        } else {
            alt = std::stod(value);
        }
        count++;
    }

    return Coord(lat, lon, alt);
}