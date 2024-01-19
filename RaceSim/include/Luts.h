/* Classes for three types of lookup tables:
   1. Efficiency lookup table csv object. The table can only have doubles or integers
   at each position. The first row and first column serve as the lookup keys and index the table
   for a certain value

   2, Regular lookup table. Table indexed by a certain row and column number.

   3. Weather lookup table. The first two columns represent the latitude and longitude respectively, and the
   first row represents a series of timestamps. Index using a lat/lon key along with a timestep key in order to 
   obtain an irradiance or wind value.
 */

#ifndef LUTS_H
#define LUTS_H

#include <stdio.h>
#include <vector>
#include <units.h>
#include <stdlib.h>
#include <string>

/* Represents a standard lookup table */
class Lut {
private:
    /* CSV Stored as a matrix */
    std::vector<std::vector<double>> values;

    /* Dimensions of the csv */
    size_t num_rows;
    size_t num_cols;

public:
    /* Index the LUT to retrieve a value */
    double get_value(size_t row_idx, size_t col_idx);

    Lut(std::string lut_path);
    Lut();
};

/* Represents an efficiency lookup table */
class Eff_Lut {
private:
    /* Index values that run along the first row of the csv */
	std::vector<double> column_values;

    /* Index values that run along the first column of the csv */
	std::vector<double> row_values;

    /* Entire csv stored as a matrix */
	std::vector<std::vector<double>> values;

    /* Dimensions of the csv */
    size_t num_rows;
    size_t num_cols;

public:
    /* Retrieve a value from the lookup table. Note that row_value and column_value indicate
       a key. If there is no matching key, the indexing takes place at the largest value
       that is smaller than both keys. */
    double get_value(double row_value, double column_value);

    /* Load a csv upon construction */
    Eff_Lut(std::string lut_path);
    Eff_Lut();
};

/* Represents a forecast lookup table */
class Forecast_Lut {
private:
    /* LUT stored as a matrix */
    std::vector<std::vector<double>> forecast_values;

    /* Coordinates used to index the lookup table */
    std::vector<ForecastCoord> forecast_coords;

    /* Timesteps used to index the lookup table as unix epoch times */
    std::vector<uint64_t> forecast_times;

    /* Dimensions of the csv */
    size_t num_rows;
    size_t num_cols;

public:
    /* Load a csv upon construction */
    Forecast_Lut(std::string lut_path);
    Forecast_Lut();

    /* Get a certain value with lat/lon and unix time as keys. Uses the closest keys */
    double get_value(ForecastCoord coord, time_t time);

    /* Caches for faster accessing */
    int row_cache;
    int column_cache;

    /* Directly indexes the csv to return a value */
    double get_value_with_cache();

    /* Updates the index_cache struct using new keys */
    void update_index_cache(ForecastCoord coord, time_t time);

    /* Reset the cache variables */
    inline void reset_caches() { row_cache = 0; column_cache = 0;}
};

#endif
