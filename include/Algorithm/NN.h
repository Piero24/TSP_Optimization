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

/**
 * @brief Optimizes a solution by removing intersections
 * 
 * @param result Pointer to the array with the inizial solution that will be optimized. 
 *  Will contain the optimized solution after the function ends.
 * @param cost Pointer to the variable to store the cost of the solution.
 * @param inst Pointer to the instance structure.
 * @param firstNode The starting node for the algorithm.
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int optimization(int* result, double* cost, instance* inst);

/**
 * @brief Optimizes a solution by searching in the neighborhood of solutions (in terms of
 *  different edges). Uses tabu search to overcome local minimums
 * 
 * @param result Pointer to the array with the inizial solution that will be optimized. 
 *  Will contain the optimized solution after the function ends.
 * @param cost Pointer to the variable to store the cost of the solution.
 * @param inst Pointer to the instance structure.
 * @param firstNode The starting node for the algorithm.
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int tabuSearch(int* result, double* cost, instance* inst);

#endif /* NN_H */