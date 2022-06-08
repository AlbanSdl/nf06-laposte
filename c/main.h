#include <stdio.h>

typedef struct {int capacity ; int battery ; float rechargetps;} Car;
typedef struct {int id; float distances[]} Node;

/**
 * @brief Test
 * 
 * @param nodes 
 * @param nbnodes 
 * @param cars 
 * @param nbcar 
 * @return int* 
 */
int* compute_path(Node nodes[], int nbnodes, Car cars[], int nbcar);