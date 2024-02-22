#include <route.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <cassert>
#include <fstream>
#include <units.h>
#include <geography.h>
#include <utilities.h>
#include <limits>
#include <config.h>
#include <Globals.h>
#include <route.h>

/* Getters */
uint32_t Route::get_num_segments() const { return num_segments; }
std::vector<std::pair<uint32_t, uint32_t>> Route::get_segments() const {return segments;}
std::vector<double> Route::get_segment_lengths() const {return segment_lengths;}
std::unordered_set<size_t> Route::get_control_stops() const {return control_stops;}
std::vector<Coord> Route::get_route_points() const {return route_points;}
uint32_t Route::get_num_points() const {return num_points;}

/* Setters */
void Route::set_num_segments(uint32_t num) {num_segments = num;}
void Route::set_segments(std::vector<std::pair<uint32_t, uint32_t>> seg) {segments = seg;}
void Route::set_segment_lengths(std::vector<double> seg_lengths) {segment_lengths = seg_lengths;}
void Route::set_control_stops(std::unordered_set<size_t> c_stops) {control_stops = c_stops;}
void Route::set_route_points(std::vector<Coord> new_route_points) {route_points = new_route_points;}

/* Load the base route */
Route::Route() {
	Config* params = Config::get_instance();
	std::string route_path = params->get_base_route_path();
	std::fstream base_route(route_path);
	assert(base_route.is_open() && "File not found...");
	route_length = 0.0;
	Coord last_coord;
	
	bool first_coord = true;
	int counter=0;
	while (!base_route.eof()) {
		std::string line;
		base_route >> line;
		std::stringstream linestream(line);

		while (!linestream.eof() && !linestream.str().empty()){
			counter++;
			std::string cell;
			Coord coord{};

			std::getline(linestream, cell, ',');
			assert(isDouble(cell) && "Value is not a number.");
			coord.lat = std::stod(cell);

			std::getline(linestream, cell, ',');
			assert(isDouble(cell) && "Value is not a number.");
			coord.lon = std::stod(cell);

			std::getline(linestream, cell, ',');
			assert(isDouble(cell) && "Value is not a number.");
			coord.alt = std::stod(cell);

			route_points.emplace_back(coord);
			
			if (!first_coord) {
				route_length = route_length + get_distance(last_coord, coord);
			} else {
				first_coord = false;
			}

			last_coord = coord;
		}
	}

	/* Segment the route */
	if (Config::get_instance()->get_optimizer() == "Constant") {
		segment_route_uniform(route_length);
	}

	num_points = route_points.size();
	control_stops = Config::get_instance()->get_control_stops();
	spdlog::info("Loaded base route with " + std::to_string(num_points) + " coordinates");
}

/* Segment a route into uniform lengths */
void Route::segment_route_uniform(double length) {
	assert(route_points.size() > 0 && "Route not loaded yet");
	size_t num_points = route_points.size();

	double current_segment_distance = 0.0;
	double difference = 0.0;
	double last_difference = std::numeric_limits<double>::max();
	std::pair<uint32_t, uint32_t> segment_indices = {0,0};

	for (size_t idx=0; idx<num_points-1; idx++) {
		Coord coord_one = route_points[idx];
		Coord coord_two = route_points[idx+1];

		segment_indices.second = idx+1;
		double distance = get_distance(coord_one, coord_two);
		current_segment_distance += distance;

		difference = std::abs(length - current_segment_distance);

		if (difference > last_difference) {
			segment_indices.second = idx;
			segments.push_back(segment_indices);
			segment_lengths.push_back(current_segment_distance);

			segment_indices = {idx, idx+1};
			current_segment_distance = distance;
			difference = std::abs(length - current_segment_distance);
		}

		last_difference = difference;
	}

	if (segment_indices.first != segment_indices.second) {
		segments.push_back(segment_indices);
		segment_lengths.push_back(current_segment_distance);
	}
	
	num_segments = segments.size();
	return;
}
