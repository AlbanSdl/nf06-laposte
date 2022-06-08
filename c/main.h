#include <stdio.h>

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
     * @brief The number of km available after one minute of recharging
     */
    float rechargetps;
} Car;

typedef struct {
    /** 
     * @brief The node id
     */
    int id;
    /**
     * @brief The distance to the other nodes in km
     */
    float distances[];
} Node;

typedef struct {
    int* nodes;
    int nodeCount;
    float time;
} RoutedVehicle;

/**
 * @brief Test
 * 
 * @param nodes 
 * @param nbnodes 
 * @param cars 
 * @param nbcar 
 * @return RoutedVehicle* 
 */
void compute_path(Node nodes[], int nbnodes, Car cars[], int nbcar, RoutedVehicle* result);