#include <stdio.h>

/**
 * @brief This is a car which can be used for the simulation.
 */
typedef struct {
    /**
     * @brief The vehicle capacity in number of customers
     */
    int capacity;
    /**
     * @brief The vehicle battery capacity in km
     */
    int battery;
    /**
     * @brief The vehicle overall speed in km/h
     */
    float speedkmh;
    /**
     * @brief The number of km available after one hour of rufueling
     */
    float rechargetps;
} Car;

/**
 * @brief This is a node (a customer location) that must be included in a path
 */
typedef struct {
    /** 
     * @brief The node id
     */
    int id;
    /**
     * @brief The distance to the other nodes in km. This is an array and the index is the node id.
     */
    float* distances;
} Node;

/**
 * @brief This structure is the data type returned when computing the paths to use
 */
typedef struct {
    /**
     * @brief The array of node ids in the order to use to reach the destination
     */
    int* nodes;
    /**
     * @brief The number of nodes in the path (including the warehouse - the first starting point is not included)
     */
    int nodeCount;
    /**
     * @brief The number of deliveries executed by this car
     */
    int deliveries;
    /**
     * @brief The distance in km of the path
     */
    float distance;
    /**
     * @brief The time needed to reach the destination (in hours)
     */
    float time;
} RoutedVehicle;

/**
 * @brief Computes the paths of the given vehicle to reach the given nodes
 * 
 * @param nodes the nodes (customers) to reach
 * @param nbnodes the length of the nodes array
 * @param cars the cars to use to reach the nodes
 * @param nbcar the length of the cars array
 * @param result the result of this function, an array of RoutedVehicle, ie. the path of each vehicle
 */
void compute_path(Node nodes[], int nbnodes, Car cars[], int nbcar, RoutedVehicle* result);