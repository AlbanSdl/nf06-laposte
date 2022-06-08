#include "main.h"
#include <stdlib.h>

typedef struct {
    int id;
    float distance;
} DistanceRef;

typedef enum {
    DELIVERED, PENDING, ERRORED
} Status;

DistanceRef* get_closest_neighbours(Node *node, int nodeCount) {
    DistanceRef* neighbours = malloc(sizeof(DistanceRef) * nodeCount);
    
    for (int i = 0; i < nodeCount; i++) {
        DistanceRef ref = {
            id: i,
            distance: node->distances[i]
        };
        neighbours[i] = ref;
    }

    // Sort distances
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
    sort_cars_by_battery(cars, nbcar);

    printf("Sorted vehicles");

    Status status[nbnodes];
    for (int i = 0; i < nbnodes; i++)
        status[i] = PENDING;
    int remainingNodes = nbnodes - 1;

    Node warehouse = nodes[0];
    DistanceRef* warehouse_distances = get_closest_neighbours(&warehouse, nbnodes);

    // RoutedVehicle* vehicles = malloc(sizeof(RoutedVehicle) * nbcar);

    for (int i = 0; i < nbcar; i++) {
        RoutedVehicle vehicle = {
            nodes: malloc(sizeof(int)),
            time: 0,
            nodeCount: 0
        };
        result[i] = vehicle;
    }

    while (remainingNodes > 0) {
        
        printf("Remaining nodes %d\n", remainingNodes);

        for (int i = nbcar - 1; i >= 0; i--) {
            int* deliveredNodes;
            float time;
            int deliveryCount = 0;

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
                        time += node->distances[0] * vehicle->speedkmh;
                        deliveredNodes = malloc(sizeof(int) * 1);
                        deliveredNodes[0] = ref->id;
                        break;
                    } else {
                        // We can't use this node
                        status[ref->id] = ERRORED;
                        remainingNodes--;
                    }
                }
            }

            printf("Starting at %d\n", deliveredNodes[0]);

            // Then we go to the closest node
            while (deliveryCount < vehicle->capacity) {
                Node currentNode = nodes[deliveredNodes[deliveryCount - 1]];
                DistanceRef* ref = get_closest_neighbours(&currentNode, nbnodes);
                short found = 0;

                for (int i = 1; i < nbnodes; i++) {
                    if (status[ref->id] == PENDING) {
                        Node* node = &nodes[ref->id];
                        if (node->distances[0] <= battery - ref->distance) {
                            battery -= node->distances[0];
                            status[ref->id] = DELIVERED;
                            remainingNodes--;

                            deliveryCount++;
                            time += node->distances[0] * vehicle->speedkmh;
                            deliveredNodes = realloc(deliveredNodes, sizeof(int) * deliveryCount);
                            deliveredNodes[deliveryCount - 1] = ref->id;
                            found = 1;
                            break;
                        }
                    }
                }

                if (found == 0) break;
            }

            deliveredNodes = realloc(deliveredNodes, sizeof(int) * (1 + deliveryCount));
            deliveredNodes[deliveryCount] = 0;

            if (deliveryCount > 0) {
                RoutedVehicle rvh = result[i];
                if (rvh.nodeCount > 0) {
                    // We have to refuel the vehicle
                    time += vehicle->battery / vehicle->rechargetps;
                }

                rvh.nodes = realloc(rvh.nodes, sizeof(int) * (rvh.nodeCount + deliveryCount + 1));
                for (int i = 0; i < deliveryCount; i++)
                    rvh.nodes[rvh.nodeCount + i] = deliveredNodes[i];
                rvh.nodeCount += deliveryCount;
                rvh.time += time;
            }
        }

        // All vehicles went for one travel
        // We reiterate that (if needed)
    }

    printf("FROM C: %d cars\n", nbcar);
    for (int i = 0; i < nbcar; i++) {
        printf("FROM C: car %d with %d nodes\n", i, result[i].nodeCount);
    }
}