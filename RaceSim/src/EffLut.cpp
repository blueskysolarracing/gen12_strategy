#include <Luts.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <cassert>
#include <assert.h>
#include <sstream>
#include <string.h>
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

