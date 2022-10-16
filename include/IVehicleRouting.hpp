/**
 *  @file   IVehicleRouting.hpp
 *  @brief  Core business logic for VRPP.
 *  @author Vikram Bhattacharjee
 *  @date  2022-02-20
 ***********************************************/

#pragma once
#include "utilities.hpp"
#include "types.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::ordered_json;

/**
 * The classes below hold the main business logic
 * behind route selection by Djikstra's algorithm.
 * The method showPossibleTrips() is the driver 
 * for the route calculation methods. 
 */
class RoutesInterface
{
public:
    virtual void showPossibleTrips() = 0;
    virtual ~RoutesInterface() {};
};

class Routes : public RoutesInterface
{
public:
    Routes(std::string path_)
    {
        path = path_;
    }

    ~Routes()
    {
        map.clear();
        path.clear();
    }

    /// <summary>
    /// showPossibleTrips calls two methods. 
    /// createPickupCustomerGraph and calculateTrips in sequence.
    /// </summary>
    void showPossibleTrips() 
    {
        createPickupCustomerGraph();
        calculateTrips();
    }

    /// <summary>
    /// createPickupCustomerGraph creates two data structures. 
    /// A heap of Pickups and a mapping between pickups and 
    /// their customers.
    /// </summary>
    void createPickupCustomerGraph()
    {
        try
        {
            // open JSON file
            std::ifstream file(path, std::ifstream::in);
            json routeDetails;
            file >> routeDetails;
            std::pair<double, double> origin_ = std::make_pair(routeDetails["startLocation"]["Lat"], routeDetails["startLocation"]["Lng"]);
            Point* origin = new Point(origin_, " ", " ", nullptr);

            for (int i = 0; i < routeDetails["points"].size(); i++)
            {
                auto points = routeDetails["points"][i];
                // collect coordinates for current point
                std::pair<double, double> coordinates_ = std::make_pair(points["lat"], points["lng"]);
                // initialize metrics for creating pickup, customer graph
                Point* current_pickup = nullptr;
                Customer* customer_ = nullptr;
                //pickups have offloading set to false
                if (points["isOffloading"] == false)
                {
                    // start collecting customers per pickup
                    std::string Id = points["id"];
                    std::string parentPointId = points["parentPointId"];
                    current_pickup = new Point(coordinates_, Id, parentPointId, origin);
                    pickups.push(current_pickup);

                    // iterate over the set of points again to pick customers..Still O(n^2) because there is 
                    // no certainty of order within input data
                    for (int k = 0; k < routeDetails["points"].size(); k++)
                    {
                        if ((routeDetails["points"][k]["isOffloading"] == true && routeDetails["points"][k]["parentPointId"] == Id))
                        {
                            double distance = calculate_distance(coordinates_.first, coordinates_.second, routeDetails["points"][k]["lat"], routeDetails["points"][k]["lng"]);
                            customer_ = new Customer(std::make_pair(routeDetails["points"][k]["lat"], routeDetails["points"][k]["lng"]), routeDetails["points"][k]["id"], distance);
                            map[current_pickup].push(customer_);
                        }
                    }
                }
            }
        }
        catch (...)
        {
            std::cerr << "Run time error\n";
        }
    }

    /// <summary>
    /// calculateTrips computes the trips for the 
    /// Vehicle Routing problem with pickups and displays 
    /// json output of the result decribed in the problem.
    /// </summary>
    void calculateTrips()  
    {
        try
        {
            std::ifstream file(path, std::ifstream::in);
            json routeDetails;
            file >> routeDetails;
            // calcuate how many combination of trips are possible from nmber of trucks
            size_t plans = factorial(routeDetails["vehicles"].size());
            json output = json::array();
            for (size_t p = 0; p < plans; p++)
            {
                // for every plan we need to have the old pickups and graph for processinf
                auto pickups_ = pickups;
                auto map_ = map;

                json driverTrip;
                for (size_t i = 0; i < routeDetails["vehicles"].size(); i++)
                {
                    json tripStats = json::array();
                    double distance = 0;
                    if (!pickups_.empty())
                    {
                        Point* starting_point = pickups_.top();
                        pickups_.pop();

                        // set the starting pickup location
                        json start
                        {
                            {"pointId", "Start"},
                            {"lat", starting_point->coordinates.first},
                            {"lng", starting_point->coordinates.second},
                            {"parentPointId", ""}
                        };

                        tripStats.push_back(start);
                        vector<Point*>& trips = Container(pickups_);

                        // iterate through trips
                        for (auto itr = trips.begin(); itr != trips.end(); itr++)
                        {
                            // add nearest pickup from routes available on Pickup queue
                            Point* currentPickup = *itr;
                            // set the starting pickup location
                            json pickupStore
                            {
                                {"pointId", currentPickup->pointId},
                                {"lat", currentPickup->coordinates.first},
                                {"lng", currentPickup->coordinates.second},
                                {"parentPointId", currentPickup->parentId}
                            };

                            distance += calculate_distance(currentPickup->coordinates.first, currentPickup->coordinates.second, starting_point->coordinates.first, starting_point->coordinates.second);
                            tripStats.push_back(pickupStore);
                            std::pair<double, double> previousCustomer = std::make_pair(currentPickup->coordinates.first, currentPickup->coordinates.second);

                            while (!map_[*itr].empty())
                            {
                                // pick customer nearest to pickup
                                // graph is of the form <pickup point, priority queue<customer>>
                                // ordered by distance from th pickup location
                                Customer* currentCustomer = map_[*itr].top();
                                json customerStore
                                {
                                    {"pointId", currentCustomer->pointId},
                                    {"lat", currentCustomer->coordinates.first},
                                    {"lng", currentCustomer->coordinates.second},
                                    {"parentPointId", currentPickup->pointId}
                                };

                                distance += calculate_distance(currentCustomer->coordinates.first, currentCustomer->coordinates.second, previousCustomer.first, previousCustomer.second);
                                tripStats.push_back(customerStore);
                                // erase the customer after its added and continue
                                map_[*itr].pop();
                                // save creds to previousCustomer for updating distance
                                previousCustomer = std::make_pair(currentCustomer->coordinates.first, currentCustomer->coordinates.second);
                            }
                        }
                    }

                    json tripSummary{ "distanceInMeter", distance * 1000 };
                    json TripId = { "tripId" , "T" + std::to_string(i + 1) };
                    json VehicleType = { "VehicleType" , routeDetails["vehicles"][i]["Type"] };
                    json Routes = { "trip" , tripStats };

                    driverTrip = {
                        TripId,
                        VehicleType,
                        {"summary", tripSummary},
                        Routes
                    };

                }

                json Plan = { "planId" , p < 10 ? "0" + std::to_string(p + 1) : std::to_string(p + 1) };
                json PlannedTrip = { "trips", driverTrip };
                output.push_back({ Plan,PlannedTrip });
            }
            std::cout << output << std::endl;
            return;
        }
        catch (...)
        {
            std::cerr << "Run time error\n";
        }
    }
private:
    // read in the json file
    std::string path;
    friend class Point;
    friend class Customer;
    priority_queue<Point*, vector<Point*>, Point::comp> pickups;
    unordered_map<Point*, priority_queue<Customer*, vector<Customer*>, Customer::comp>> map;
};
