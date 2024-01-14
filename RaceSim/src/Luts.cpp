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

/* Load an efficiency csv lookup table */
Eff_Lut::Eff_Lut(std::string lut_path) {
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

    num_rows = column_values.size();
    num_cols = row_values.size();
}

Eff_Lut::Eff_Lut() {
	std::cout << "Please provide a the path to the LUT." << std::endl;
	return;
}

double Eff_Lut::get_value(double row_value, double column_value) {
    size_t row = 0;
    size_t col = 0;

    for (; row < num_rows; row++) {
        if (row_value == row_values[row] || row_value > row_values[row]) {
            break;
        }
    }

    for (; col < num_cols; col++) {
        if (column_value == column_values[col] || column_value > column_values[col]) {
            break;
        }
    }

    return values[row][col];
}

Forecast_Lut::Forecast_Lut(std::string lut_path) {
	std::fstream file(lut_path);
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
		int month = (temp_time % 100)-1;
		temp_time /= 100;
		int year = temp_time + 100;
		tm forecast_time = tm({seconds, minutes, hours, days, month, year, 0, 0, -1});
		forecast_times.push_back(mktime(&forecast_time)); // times in the forecast csv are already in utc time
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

		int column_counter = 0;
		while (!file_linestream.eof()){

			std::getline(file_linestream, cell, ',');
			assert(isDouble(cell) && "Value is not a number.");
			forecast_values[row_counter][column_counter] = std::stod(cell);
			column_counter++;
		}

		row_counter++;
	}

	num_rows = forecast_coords.size();
	num_cols = forecast_times.size();

	row_cache = 0;
	column_cache = 0;
}

Forecast_Lut::Forecast_Lut() {
	std::cout << "Please provide a the path to the LUT." << std::endl;
	return;
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

    return forecast_values[row_key][col_key];
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
	return forecast_values[row_cache][column_cache];
}