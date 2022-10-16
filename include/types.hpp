/**
 *  @file   types.hpp
 *  @brief  Class definitions for different 
 *          entities that solve VRPP. The main two 
 *          attributes are Point and Customer.
 *  @author Vikram Bhattacharjee
 *  @date  2022-10-15
 ***********************************************/


#pragma once
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>   
#include <unordered_map>

using namespace std;

// STL priority queues does not have iterators.
// This workaround creates and exposes an iterable container class 
// for the private data structure within priority queue.
template <class T, class S, class C>
S& Container(priority_queue<T, S, C>& q)
{
    struct queue : private priority_queue<T, S, C>
    {
        static S& Container(priority_queue<T, S, C>& q)
        {
            return q.* & queue::c;
        }
    };
    return queue::Container(q);
}

/// <summary>
/// Point class
/// Holds information of pickups.
/// </summary>
class Point
{
public:
    Point(std::pair<double, double> coordinates_, std::string pointId_, std::string parentId_, Point* origin)
    {
        pointId = pointId_;
        coordinates.first = coordinates_.first;
        coordinates.second = coordinates_.second;
        parentId = parentId_;
        set_distance(origin);
    }

    struct comp
    {
    public:
        bool operator()(Point* a, Point* b)
        {
            return a->distance >= b->distance;
        }
    };

    void set_distance(Point* origin)
    {
        if (origin == nullptr)
        {
            distance = 0;
            return;
        }
        distance = calculate_distance(coordinates.first, coordinates.second, origin->coordinates.first, origin->coordinates.second);
    }

    ~Point() {}
public:
    friend class Customer;
    std::pair<double, double> coordinates;
    double distance;
    std::string pointId;
    std::string parentId;
};

/// <summary>
/// Customer Class 
/// Holds information of customer.
/// </summary>
class Customer
{
public:
    Customer(std::pair<double, double> coordinates_, std::string pointId_, double distance_)
    {
        pointId = pointId_;
        coordinates.first = coordinates_.first;
        coordinates.second = coordinates_.second;
        distance = distance_;
    }

    struct comp
    {
    public:
        bool operator()(Customer* a, Customer* b)
        {
            return a->distance >= b->distance;
        }
    };

    ~Customer() {}

public:
    std::pair<double, double> coordinates;
    std::string pointId;
    double distance;
};
