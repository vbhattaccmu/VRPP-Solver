# A VRPP-Solver in C++
This repository presents a solution to the Vehicle Routing with Pickups 
problem with C++-11. 
- The program takes in a Sample_Input.json which describes a schema of `Point` locations on a route with their respective labels (`Pickup`: A `Point` where the vehicle   picks up goods from and `Customer`: A Point where the vehicle drops goods to.) and calculates an optimal route for all pickup locations given the total number of       vehicles. 
- The problem uses a modification of Djikstra's algorithm to compute the routes. The final result is dumped as beautified JSON as shown in the `Sample Results` section 
  below.
- This repositoy uses nlohmann-json as a dependency and can be installed through this [link](https://github.com/nlohmann/json).
For more information on VRPP , please refer to this [link](https://core.ac.uk/download/19477982.pdf).

## Class Definition and relevant methods

1. **Point Class** 
   - This generic class holds the attributes of a point.Predominantly used
     to create the queue for pickups. The following attributes are as follows.
     pointId,
     latitude,
     longitude,
     parentId
     
1. **Customer Class**
   - This class holds the attributes of the customer locationd.The following attributes 
     are as follows.Customer is a friend of Point class.
     pointId,
     latitude,
     longitude,
     distance [distance from parent pickup]

## Methods that solve the Routing with Pickups problem 

1.  `createPickupCustomerGraph()`
    - createPickupCustomerGraph creates a graph between pickup locations and customer locations. 
    - The initial pickup location is chosen on basis of the nearest distance from starting Point provided 
      in the problem.This is achieved by using a priority queue as well.
    - The underlying data structure is an unordered_map where the key is the pointer to a Point object 
      and a priority queue of its Customers.The Customers are inserted on the queue on basis of 
      their distance from their parent pickup location.

2.  `calculateTrip()`
    - Outlines all the rules laid out in the pseudocode and dumps a JSON output to screen for all the 
      trips similar to the output pattern outlined in the problem. 

These two methods are accessed through an interface RoutesInterface().

## Running the project

The source code needs a JSON file as an input. A sample input with the schema on which the problem is built is provided.

## Sample Results

On executing the binary, 
> VehicleRouting.exe 

the program will give a dump  
```
[{"planId":"01","trips":{"tripId":"T1","VehicleType":"pickup_truck","summary":["distanceInMeter",108839.36192330891],"trip":[{"pointId":"Start","lat":25.119234619804637,"lng":55.32858343144892,"parentPointId":""},{"pointId":"AC1","lat":25.169966760399298,"lng":55.33797361242132,"parentPointId":""},{"pointId":"Entity19","lat":25.160271153839584,"lng":55.373437534844,"parentPointId":"AC1"},{"pointId":"Entity5","lat":25.128027301292477,"lng":55.40456414792345,"parentPointId":"AC1"},{"pointId":"Entity7","lat":25.02078489164754,"lng":55.229514113467374,"parentPointId":"AC1"},{"pointId":"Entity9","lat":25.00273809443353,"lng":55.41489578748308,"parentPointId":"AC1"},{"pointId":"Entity13","lat":25.016245317452828,"lng":55.12399970005008,"parentPointId":"AC1"},{"pointId":"Entity3","lat":25.025137653808518,"lng":55.0982393028216,"parentPointId":"AC1"},{"pointId":"Entity1","lat":24.958398875188788,"lng":55.09576756643379,"parentPointId":"AC1"},{"pointId":"Entity11","lat":24.997941006864245,"lng":55.047637402389086,"parentPointId":"AC1"},{"pointId":"Entity17","lat":24.973591784101433,"lng":55.06621871082559,"parentPointId":"AC1"},{"pointId":"Entity15","lat":24.93668063960516,"lng":55.098062069246254,"parentPointId":"AC1"}]}}]
```


