/** 
 *  @file   VehicleRouting.cpp 
 *  @brief  A simple VRPP Solver in C++.
 *  @author Vikram Bhattacharjee 
 *  @date  2022-10-15
 ***********************************************/

#include "routeFactory.hpp"

/**
 * Client side interface.
 */
void ClientRoutes(AbstractRouteFactory& factory) {
    RoutesInterface* route = factory.createNewRouteSets();
    route->showPossibleTrips();
    delete route;
}

int main() 
{
    AbstractRouteFactory* factory = new ConcreteRouteFactory("Sample Input.json");
    ClientRoutes(*factory);
    delete factory;
    return EXIT_SUCCESS;
}
