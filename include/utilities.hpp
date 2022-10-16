/**
 *  @file   utilities.hpp
 *  @brief  A general utilities workspace for solving VRPP.
 *          The methods are currently inline.
 *  @author Vikram Bhattacharjee
 *  @date  2022-10-15
 ***********************************************/
#pragma once
#include <math.h>

// Helper functions for Haversine distance.
inline double calculate_distance(double latitude1, double longitude1, double latitude2, double longitude2)
{
    double radius = double{ 6371 }; // Earth's radius

    // Get the difference between our two points then convert the difference into radians.
    double lat_delta = (latitude2 - latitude1) * (3.14 / 180);
    double lon_delta = (longitude2 - longitude1) * (3.14 / 180);
    double converted_lat1 = (latitude1) * (3.14 / 180);
    double converted_lat2 = (latitude2) * (3.14 / 180);
    double a = pow(sin(lat_delta / 2), 2) + cos(converted_lat1) * cos(converted_lat2) * pow(sin(lon_delta / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return radius * c;
}

// Factorial method for calculating number of plans.
inline size_t factorial(size_t n)
{
    return (n == 1 || n == 0) ? 1 : n * factorial(n - 1);
}

