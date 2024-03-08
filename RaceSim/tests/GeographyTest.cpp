#include <gtest/gtest.h>
#include "geography.h"
#include "units.h"

TEST(TestGeography, GetDistance) {
    /* Create coordinates */
    Coord coord_one = Coord();
    Coord coord_two = Coord();

    double distance = get_distance(coord_one, coord_two);
    double true_distance = 0.0;

    EXPECT_EQ(distance, true_distance);
}
