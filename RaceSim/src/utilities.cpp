#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <units.h>
#include <cmath>

bool isDouble(std::string str) {
	if (str[0] == '-' && str.size() >= 2) {
		return isdigit(str[1]);
	}
	return isdigit(str[0]);
}

/* Load an efficiency csv lookup table */
void EfficiencyLut::load_eff_lut(std::string lut_path) {
	std::fstream lut(lut_path);
	assert(lut.is_open() && "Unable to open efficiency lut file");
	std::string line;

	// Read first row values
	std::getline(lut, line);
	std::stringstream linestream(line);
	std::string cell;

	std::getline(linestream, cell, ','); // Remove empty cell at top left of file.

	while (!linestream.eof()) {
		std::getline(linestream, cell, ',');
		assert(isDouble(cell) && "Value is not a number.");
		double column_val = std::stod(cell);
		column_values.emplace_back(column_val);
	}

	while (!lut.eof()) {
		std::getline(lut, line);
		if (line.empty()) continue;
		std::stringstream linestream(line);

		std::getline(linestream, cell, ',');
		assert(isDouble(cell) && "Value is not a number.");
		double row_val = std::stod(cell);
		row_values.emplace_back(row_val);

		values.emplace_back(std::vector<double>());
		while (!linestream.eof()) {
			std::getline(linestream, cell, ',');
			assert(isDouble(cell) && "Value is not a number.");
			double val = std::stod(cell);
			values.back().emplace_back(val);
		}
	}
}

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
	double alt_1 = src_coord.altitude;
	double alt_2 = dst_coord.altitude;

	double alt_difference = abs(alt_1-alt_2);

	// Calculate true distance with haversine
	double true_distance = sqrt(dist_km * dist_km + alt_difference * alt_difference) * 1000.0;
	
	return true_distance;
}