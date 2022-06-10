#include "main.h"
#include <stdlib.h>

/**
 * @brief This structure aims at representing a node (and its distance) from another node.
 */
typedef struct {
    /**
     * @brief The id of the remote node
     */
    int id;
    /**
     * @brief The distance between the two nodes
     */
    float distance;
} DistanceRef;

/**
 * @brief This enum is used to represent the different states of a node
 */
typedef enum {
    /**
     * @brief States that this node has already been delivered
     */
    DELIVERED,
    /**
     * @brief States that this node has not been delivered yet
     */
    PENDING,
    /**
     * @brief States that this node cannot be reached (too far for the vehicles we can use)
     */
    ERRORED
} Status;


/**
 * @brief Retrieves all the nodes sorted by shortest distance to the given node
 * 
 * @param node the node to use as reference
 * @param nodeCount the number of nodes (in the whole problem)
 * @return DistanceRef* an array of nodes sorted by shortest distance to the given node
 */
DistanceRef* get_closest_neighbours(Node *node, int nodeCount) {
    // Allocates memory for the output
    DistanceRef* neighbours = malloc(sizeof(DistanceRef) * nodeCount);
    
    // Iterate over all the nodes to create their output form
    for (int i = 0; i < nodeCount; i++) {
        DistanceRef ref = {
            id: i,
            distance: node->distances[i]
        };
        neighbours[i] = ref;
    }

    // Sort the notes using their distances
    for (int i = 0; i < nodeCount - 1; i++) {
        float minDistance = neighbours[i].distance;
        int minIndex = i;
        for (int j = i + 1; j < nodeCount; j++) {
            if (neighbours[j].distance < minDistance) {
                minDistance = neighbours[j].distance;
                minIndex = j;
            }
        }
        // Perform permutation
        DistanceRef temp = neighbours[i];
        neighbours[i] = neighbours[minIndex];
        neighbours[minIndex] = temp;
    }

    return neighbours;
}

/**
 * @brief Sorts the cars by their battery capacity (alters the given array)
 * 
 * @param cars the array of the cars which is going to be sorted
 * @param nbcar the length of the given array
 */
void sort_cars_by_battery(Car *cars, int nbcar) {
    for (int i = 0; i < nbcar - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < nbcar; j++)
            if (cars[minIndex].battery > cars[j].battery)
                minIndex = j;
        Car temp = cars[i];
        cars[i] = cars[minIndex];
        cars[minIndex] = temp;
    }
}


void compute_path(Node nodes[], int nbnodes, Car cars[], int nbcar, RoutedVehicle* result) {
    // Sort the cars by their battery capacity
    sort_cars_by_battery(cars, nbcar);

    // Keep track of delivery status of all nodes. At the beginning, all nodes are pending
    Status status[nbnodes];
    for (int i = 0; i < nbnodes; i++)
        status[i] = PENDING;
    
    // The number of remaining deliveries to execute
    int remainingNodes = nbnodes;

    // The first node is the warehouse, we keep its reference to be able to compute
    // its distance to the other nodes later on.
    Node warehouse = nodes[0];
    DistanceRef* warehouse_distances = get_closest_neighbours(&warehouse, nbnodes);

    // We initialize the output data
    for (int i = 0; i < nbcar; i++) {
        RoutedVehicle vehicle = {
            nodes: malloc(sizeof(int)),
            time: 0,
            nodeCount: 0,
            deliveries: 0,
            distance: 0,
        };
        result[i] = vehicle;
    }

    // We keep adding deliveries to the vehicles until all the nodes are delivered
    while (remainingNodes > 0) {

        // We iterate over the cars to find the one that can deliver the next nodes
        for (int i = nbcar - 1; i >= 0; i--) {
            int* deliveredNodes;
            float time;
            int deliveryCount = 0;
            int deliveries = 0;
            float distance = 0;

            // We use this vehicle:
            Car* vehicle = &cars[i];
            float battery = vehicle->battery;

            // We will go to this node (at first)
            for (int i = nbnodes - 1; i > 0; i--) {
                DistanceRef* ref = &warehouse_distances[i];
                if (status[ref->id] == PENDING) {
                    // We can go to this node (and deliver the customer)
                    Node* node = &nodes[ref->id];
                    if (node->distances[0] <= battery / 2) {
                        // We can use this node
                        battery -= node->distances[0];
                        status[ref->id] = DELIVERED;
                        remainingNodes--;

                        deliveryCount++;
                        time += node->distances[0] / vehicle->speedkmh;
                        distance += node->distances[0];
                        deliveries++;
                        deliveredNodes = malloc(sizeof(int) * deliveryCount);
                        deliveredNodes[0] = ref->id;
                        break;
                    } else {
                        // We can't use this node
                        status[ref->id] = ERRORED;
                        remainingNodes--;
                    }
                }
            }

            // Then we go to the closest node and continue until vehicle is full or all nodes are delivered
            while (deliveryCount < vehicle->capacity && remainingNodes > 0) {
                Node currentNode = nodes[deliveredNodes[deliveryCount - 1]];
                DistanceRef* ref = get_closest_neighbours(&currentNode, nbnodes);
                short found = 0;

                // We iterate over the closest nodes to find one that can be used
                for (int i = 1; i < nbnodes; i++) {
                    DistanceRef distant = ref[i];
                    if (status[distant.id] == PENDING) {
                        Node* node = &nodes[distant.id];
                        if (node->distances[0] <= battery - distant.distance) {
                            // This node can be used so we use it
                            battery -= distant.distance;
                            status[distant.id] = DELIVERED;
                            remainingNodes--;

                            deliveryCount++;
                            time += node->distances[0] / vehicle->speedkmh;
                            distance += node->distances[0];
                            deliveries++;
                            deliveredNodes = realloc(deliveredNodes, sizeof(int) * deliveryCount);
                            deliveredNodes[deliveryCount - 1] = distant.id;
                            found = 1;
                            break;
                        }
                    }
                }

                // No node can be used, we exit the loop (and we end the path of this vehicle)
                if (found == 0) break;
            }

            // We add the path for the vehicle to come back to the warehouse
            deliveredNodes = realloc(deliveredNodes, sizeof(int) * (1 + deliveryCount));
            deliveredNodes[deliveryCount] = 0;
            deliveryCount++;

            // If the vehicle has delivered packages in this run, we add them to the output
            if (deliveryCount > 1) {
                RoutedVehicle* rvh = &result[i];

                // This is not the first run, we need to add refueling time
                if (rvh->nodeCount > 0) {
                    // We have to refuel the vehicle
                    time += vehicle->battery / vehicle->rechargetps;
                }

                rvh->nodes = realloc(rvh->nodes, sizeof(int) * (rvh->nodeCount + deliveryCount));
                for (int i = 0; i < deliveryCount; i++)
                    rvh->nodes[rvh->nodeCount + i] = deliveredNodes[i];
                rvh->nodeCount += deliveryCount;
                rvh->time += time;
                rvh->deliveries += deliveries;
                rvh->distance += distance;
            }
        }

        // All vehicles went for one travel
        // We reiterate that (if needed)
    }
}