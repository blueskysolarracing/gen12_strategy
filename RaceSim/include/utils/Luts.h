/* Classes for three types of lookup tables:

   1. Regular lookup table. Table indexed by a certain row and column index starting from 0.

   2. Efficiency lookup table csv object. The first row and first column serve as the lookup keys. Index the
   LUT using these keys

   3. Weather lookup table. The first two columns represent the latitude and longitude respectively, and the
   first row represents a series of timestamps. Index using a lat/lon key along with a timestep key in order to 
   obtain an irradiance or wind value.
 */

#ifndef LUTS_H
#define LUTS_H

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
#include "spdlog/spdlog.h"
#include <stdio.h>
#include <vector>
#include <units.h>

/* Base LUT */
template <typename T>
class Base_Lut {
protected:
    /* Relative path to LUT */
    std::string lut_path;

    /* LUT stored as a matrix */
    std::vector<std::vector<T>> values;

    /* Dimensions of the LUT */
    size_t num_rows;
    size_t num_cols;

    virtual void load_LUT() = 0;
public:
    /* Only stores the relative path to the LUT */
    Base_Lut(const std::string path);

    /* We let the derived LUTs implement their own lookup functionality */
};

/* Represents a standard double type lookup table */
class Basic_Lut : public Base_Lut<double> {
private:
    void load_LUT() override;
public:
    /* Index the LUT to retrieve a value */
    double get_value(size_t row_idx, size_t col_idx);

    /* Load csv upon construction */
    Basic_Lut(const std::string path);
};

/* Represents an efficiency lookup table of double values */
class Eff_Lut : public Base_Lut<double> {
private:
    /* Index values that run along the first row of the csv */
	std::vector<double> column_values;

    /* Index values that run along the first column of the csv */
	std::vector<double> row_values;

    void load_LUT() override;
public:
    /* Retrieve a value from the lookup table. Note that row_value and column_value indicate
       a key. If there is no matching key, the indexing takes place at the largest value
       that is smaller than both keys. */
    double get_value(double row_value, double column_value);

    /* Load a csv upon construction */
    Eff_Lut(const std::string lut_path);
};

/* Represents a forecast lookup table of double values */
class Forecast_Lut : public Base_Lut<double>{
private:

    /* Coordinates used to index the lookup table */
    std::vector<ForecastCoord> forecast_coords;

    /* Timesteps used to index the lookup table as unix epoch times */
    std::vector<time_t> forecast_times;

    void load_LUT() override;
public:
    /* Load a csv upon construction */
    Forecast_Lut(std::string lut_path);

    /* Get a certain value with lat/lon and unix time as keys. Uses the closest keys */
    double get_value(ForecastCoord coord, time_t time);

    /* Caches for faster accessing */
    int row_cache;
    int column_cache;

    /* Directly indexes the csv to return a value */
    double get_value_with_cache();

    /* Updates the index_cache struct using new keys */
    void update_index_cache(ForecastCoord coord, time_t time);

    /* Initialize the cache variables */
    void initialize_caches(ForecastCoord coord, time_t time);
    void initialize_caches(Coord coord, time_t time);
};

#endif
