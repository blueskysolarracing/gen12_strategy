#include <Luts.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <cassert>
#include <assert.h>
#include <sstream>
#include <string.h>
#include <geography.h>
#include <limits>
#include <utilities.h>
#include <date.h>
#include <config.h>
#include <Globals.h>
#include "spdlog/spdlog.h"

template <typename T>
Base_Lut<T>::Base_Lut(const std::string path) {
	lut_path = path;
}

void Basic_Lut::load_LUT() {
	std::fstream lut(this->lut_path);
	ASSERT(lut.is_open(), "Unable to open efficiency lut file " + this->lut_path);
	std::string line;
	std::string cell;

	while (!lut.eof()) {
		std::getline(lut, line);
		if (line.empty()) continue;
		std::stringstream linestream(line);

		values.emplace_back(std::vector<double>());
		while (!linestream.eof()) {
			std::getline(linestream, cell, ',');
			assert(isDouble(cell) && "Value is not a number.");
			double val = std::stod(cell);
			values.back().emplace_back(val);
		}
	}

    this->num_rows = values.size();
	ASSERT(num_rows > 0, "No data loaded in from: " + this->lut_path);
    this->num_cols = values[0].size();
	spdlog::info("Loaded LUT: " + this->lut_path);
}

double Basic_Lut::get_value(size_t row_idx, size_t col_idx) {
	ASSERT_EXIT((row_idx >= 0 && row_idx < num_rows && col_idx >= 0 && col_idx < num_cols), "Attempted index out of bounds on: " + this->lut_path);
	return values[row_idx][col_idx];
}

Basic_Lut::Basic_Lut(const std::string lut_path) : Base_Lut<double>(lut_path) {
	load_LUT();
}

void Eff_Lut::load_LUT() {
	std::fstream lut(this->lut_path);
	ASSERT(lut.is_open(), "Unable to open efficiency lut file: " + this->lut_path);	
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
		column_values.push_back(column_val);
	}

	while (!lut.eof()) {
		std::getline(lut, line);
		if (line.empty()) continue;
		std::stringstream linestream(line);

		std::getline(linestream, cell, ',');
		assert(isDouble(cell) && "Value is not a number.");
		double row_val = std::stod(cell);
		row_values.push_back(row_val);

		this->values.emplace_back(std::vector<double>());
		while (!linestream.eof()) {
			std::getline(linestream, cell, ',');
			assert(isDouble(cell) && "Value is not a number.");
			double val = std::stod(cell);
			this->values.back().emplace_back(val);
		}
	}

    this->num_cols = column_values.size();
    this->num_rows = row_values.size();
	spdlog::info("Loaded LUT: " + this->lut_path);
}

Eff_Lut::Eff_Lut(const std::string lut_path) : Base_Lut<double>(lut_path) {
	load_LUT();
}

double Eff_Lut::get_value(double row_value, double column_value) {
    size_t row = 0;
    size_t col = 0;

    for (; row < num_rows; row++) {
        if (row_value == row_values[row] || row_values[row] > row_value) {
            break;
        }
    }

    for (; col < num_cols; col++) {
        if (column_value == column_values[col] || column_values[col] > column_value) {
            break;
        }
    }

	assert(row >= 0 && row < num_rows && col >= 0 && col < num_cols);
    return values[row][col];
}

Forecast_Lut::Forecast_Lut(std::string lut_path) : Base_Lut<double>(lut_path) {
	load_LUT();
}

void Forecast_Lut::load_LUT() {
	std::fstream file(this->lut_path);
	std::string times_line;
	file >> times_line;
	std::stringstream times_stream(times_line);

	// Remove 'latitude' and 'longitude' from first 2 cols of csv input.
	std::string time;
	std::getline(times_stream, time, ',');
	std::getline(times_stream, time, ',');

	/* Create an array of the time keys */
	while (!times_stream.eof()) {
		std::getline(times_stream, time, ',');
		assert(isDouble(time) && "Time is not a number.");
		uint64_t temp_time = std::stoull(time);
		int seconds = temp_time % 100;
		temp_time /= 100;
		int minutes = temp_time % 100;
		temp_time /= 100;
		int hours = temp_time % 100;
		temp_time /= 100;
		int days = temp_time % 100;
		temp_time /= 100;
		int month = (temp_time % 100);
		temp_time /= 100;
		int year = temp_time;

		/* Construct YYYY-MM-DD HH:MM:SS string */
		std::string forecast_time = "20" + std::to_string(year) + "-" + std::to_string(month) + "-" 
			+ std::to_string(days) + " " + std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    	std::istringstream iss(forecast_time);
   		date::sys_time<std::chrono::seconds> epoch_time;
    	iss >> date::parse("%F %T", epoch_time);
		time_t local_time_t = std::chrono::system_clock::to_time_t(epoch_time);

		forecast_times.push_back(local_time_t);
	}

	int row_counter = 0;
	while (!file.eof()) {
		std::string file_line;
		file >> file_line;
		std::stringstream file_linestream(file_line);
		if (file_linestream.str().empty()) break;

		std::string cell;
		ForecastCoord coord{};
		std::getline(file_linestream, cell, ',');
		assert(isDouble(cell) && "Value is not a number.");
		coord.lat = std::stod(cell);

		std::getline(file_linestream, cell, ',');
		assert(isDouble(cell) && "Value is not a number.");
		coord.lon = std::stod(cell);

		forecast_coords.emplace_back(coord);

		std::getline(file_linestream, cell, ',');
		assert(isDouble(cell) && "Value is not a number");
		double value = std::stod(cell);
		std::vector<double> inner_vector;
		inner_vector.emplace_back(value);
		this->values.push_back(inner_vector);

		int column_counter = 0;
		while (!file_linestream.eof()){
			std::getline(file_linestream, cell, ',');
			assert(isDouble(cell) && "Value is not a number.");
			this->values[row_counter].push_back(std::stod(cell));
			column_counter++;
		}

		row_counter++;
	}

	this->num_rows = forecast_coords.size();
	ASSERT(this->num_rows > 0, "No data loaded from: " + this->lut_path);
	this->num_cols = forecast_times.size();

	row_cache = 0;
	column_cache = 0;
	spdlog::info("Loaded LUT: " + this->lut_path);
}

double Forecast_Lut::get_value(ForecastCoord coord, time_t time) {
	double row_key;
	double col_key;

	double min_distance = std::numeric_limits<double>::max();
    for (size_t row=0; row < num_rows; row++) {
        ForecastCoord forecast_coord = forecast_coords[row];
		double distance = get_forecast_coord_distance(coord, forecast_coord);
		if (distance < min_distance) {
			min_distance = distance;
			row_key = row;
		}
    }

	double min_time = std::numeric_limits<double>::max();
    for (size_t col=0; col < num_cols; col++) {
        uint64_t forecast_time = forecast_times[col];
		int time_diff = time - forecast_time;
		if (std::abs((double) time_diff) < min_time) {
			min_time = std::abs((double) time_diff);
			col_key = col;
		}
    }

	assert(row_key >= 0 && row_key < num_rows && col_key >= 0 && col_key < num_cols);
    return this->values[row_key][col_key];
}

void Forecast_Lut::initialize_caches(ForecastCoord coord, time_t time) {

	/* Initialize row cache */
	Coord forecast_coord_as_coord = Coord(coord);
	double min_distance = std::numeric_limits<double>::max();
	for (size_t i = 0; i < num_rows; i++) {
		Coord forecast_coord = Coord(forecast_coords[i]);
		double distance = get_distance(forecast_coord, forecast_coord_as_coord);
		if (distance < min_distance) {
			min_distance = distance;
			row_cache = i;
		}
	} 

	/* Initialize column cache */
	double min_time = std::numeric_limits<double>::max();
	for (size_t i=0; i<num_cols; i++) {
		time_t forecast_time = forecast_times[i];
		int time_diff = time - forecast_time;
		if (std::abs((double) time_diff) < min_time) {
			min_time = std::abs((double) time_diff);
			column_cache = i;
		}
	}
}

void Forecast_Lut::initialize_caches(Coord coord, time_t time) {

	/* Initialize row cache */
	double min_distance = std::numeric_limits<double>::max();
	for (size_t i = 0; i < num_rows; i++) {
		Coord forecast_coord = Coord(forecast_coords[i]);
		double distance = get_distance(forecast_coord, coord);
		if (distance < min_distance) {
			min_distance = distance;
			row_cache = i;
		}
	} 

	/* Initialize column cache */
	double min_time = std::numeric_limits<double>::max();
	for (size_t i=0; i<num_cols; i++) {
		time_t forecast_time = forecast_times[i];
		int time_diff = time - forecast_time;
		if (std::abs((double) time_diff) < min_time) {
			min_time = std::abs((double) time_diff);
			column_cache = i;
		}
	}
}

/* Begins searching from the specified indices */
void Forecast_Lut::update_index_cache(ForecastCoord coord, time_t time) {

	if (row_cache < num_rows-1) {
		ForecastCoord next_coord = forecast_coords[row_cache+1];
		ForecastCoord current_coord = forecast_coords[row_cache];
		
		double dist_from_current_coord = get_forecast_coord_distance(coord, current_coord);
		double dist_from_next_coord = get_forecast_coord_distance(coord, next_coord);

		row_cache = dist_from_current_coord <= dist_from_next_coord ? row_cache : row_cache+1;
	} else {
		row_cache = row_cache;
	}

	if (column_cache < num_cols-1) {
		uint64_t current_time = forecast_times[column_cache];
		uint64_t next_time = forecast_times[column_cache+1];

		uint64_t diff_time_from_current = abs((double) (time - current_time));
		uint64_t diff_time_from_next = abs((double) (time - next_time));

		column_cache = diff_time_from_current <= diff_time_from_next ? column_cache : column_cache+1;
	} else {
		column_cache = column_cache;
	}
}

double Forecast_Lut::get_value_with_cache() {
	ASSERT_EXIT((row_cache >= 0 && row_cache < this->num_rows && column_cache >= 0 && column_cache < this->num_cols),
			"Cache values for Forecast Lut outside of bounds.");
	return this->values[row_cache][column_cache];
}