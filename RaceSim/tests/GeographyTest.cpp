#include <gtest/gtest.h>
#include "geography.h"
#include "units.h"

TEST(TestGeography, GetDistanceEmptyCoord) {
    /* Create coordinates */
    Coord coord_one = Coord();
    Coord coord_two = Coord();

    double distance = get_distance(coord_one, coord_two);
    double true_distance = 0.0;

    EXPECT_EQ(distance, true_distance);
}

TEST(TestGeography, GetDistanceForecastCoord) {
    /* Create coordinates */
    ForecastCoord coord_one = ForecastCoord();
    ForecastCoord coord_two = ForecastCoord();

    double distance = get_forecast_coord_distance(coord_one, coord_two);
    double true_distance = 0.0;

    EXPECT_DOUBLE_EQ(distance, true_distance);

    coord_one = ForecastCoord(10, 20);
    coord_two = ForecastCoord(40, 150);

    distance = get_forecast_coord_distance(coord_one, coord_two);
    true_distance = 12444880;
    EXPECT_NEAR(distance,true_distance,10);
}

TEST(TestGeography, GetDistanceCoord) {
    /* Create coordinates */
    Coord coord_one = Coord(10, 20, 0);
    Coord coord_two = Coord(10, 20.01, 100);

    double distance = get_distance(coord_one, coord_two);
    double true_distance = 1099.56;

    EXPECT_NEAR(distance, true_distance, 1);

    /* Check three sample distances*/
    coord_one = Coord(30.2, 43.6, 2);
    coord_two = Coord(30.3, 43.7, 478);
    distance = get_distance(coord_one, coord_two);
    true_distance = 14701;
    EXPECT_NEAR(distance, true_distance, 1);

    coord_one = Coord(-27, 133.6, 89);
    coord_two = Coord(-27, 134, 1498);
    distance = get_distance(coord_one, coord_two);
    true_distance = 39655;
    EXPECT_NEAR(distance, true_distance, 1);

    coord_one = Coord(30.2, 43.6, -300);
    coord_two = Coord(30.3, 43.7, 478);
    distance = get_distance(coord_one, coord_two);
    true_distance = 14714;
    EXPECT_NEAR(distance, true_distance, 1);

}

TEST(TestGeography, GetBearing) {
    /* Create coordinates */
    Coord coord_one = Coord(50.06, -5.71, 0);
    Coord coord_two = Coord(58.64, -3, 0);

    double bearing = get_bearing(coord_one, coord_two);
    double true_bearing = 9.11972;

    EXPECT_NEAR(bearing, true_bearing, 0.3);

    /*Ensure altitude doesn't impact bearing*/
    coord_one = Coord(50.06, -5.71, 30000);
    coord_two = Coord(58.64, -3, 0);

    bearing = get_bearing(coord_one, coord_two);
    true_bearing = 9.11972;

    EXPECT_NEAR(bearing, true_bearing, 0.3);
}

TEST(TestGeography, GetSpeedRelativeWind) {
    /* Create inital testing values */
    double car_speed = 40;
    Wind wind = Wind();
    double car_bearing = 40;

    double relative_speed = get_speed_relative_to_wind(car_speed, car_bearing, wind);

    EXPECT_NEAR(relative_speed, 40, 0.01);

    /* Checks all angle quadrants to ensure accuracy*/
    car_speed = 40;
    car_bearing = 40;
    wind = Wind(40, 40);
    relative_speed = get_speed_relative_to_wind(car_speed, car_bearing, wind);
    EXPECT_NEAR(relative_speed, 80, 0.3);

    car_speed = 60;
    car_bearing = 10;
    wind = Wind(20, 10);
    relative_speed = get_speed_relative_to_wind(car_speed, car_bearing, wind);
    EXPECT_NEAR(relative_speed, 69.8, 0.1);

    car_speed = 60;
    car_bearing = 10;
    wind = Wind(110, 10);
    relative_speed = get_speed_relative_to_wind(car_speed, car_bearing, wind);
    EXPECT_NEAR(relative_speed, 58.26, 0.1);

    car_speed = 60;
    car_bearing = 180;
    wind = Wind(200, 10);
    relative_speed = get_speed_relative_to_wind(car_speed, car_bearing, wind);
    EXPECT_NEAR(relative_speed, 69.4, 0.1);

    car_speed = 80;
    car_bearing = 210;
    wind = Wind(310, 10);
    relative_speed = get_speed_relative_to_wind(car_speed, car_bearing, wind);
    EXPECT_NEAR(relative_speed, 78.26, 0.1);

    car_speed = 80;
    car_bearing = 210;
    wind = Wind(310, 5);
    relative_speed = get_speed_relative_to_wind(car_speed, car_bearing, wind);
    EXPECT_NEAR(relative_speed, 79.13, 0.1);

    car_speed = 60;
    car_bearing = 10;
    wind = Wind(300, 10);
    relative_speed = get_speed_relative_to_wind(car_speed, car_bearing, wind);
    EXPECT_NEAR(relative_speed, 63.42, 0.1);
}