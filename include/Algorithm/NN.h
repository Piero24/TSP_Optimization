#ifndef NN_H
#define NN_H

#include "../tsp.h"

/**
 * @brief Applies the nearest neighbor algorithm to find the solution for the given instance.
 * 
 * @param result Pointer to the array to store the resulting solution.
 * @param cost Pointer to the variable to store the cost of the solution.
 * @param inst Pointer to the instance structure.
 * @param firstNode The starting node for the algorithm.
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int nearestNeighbor(int* result, double* cost, instance* inst, int firstNode);

#endif /* NN_H */