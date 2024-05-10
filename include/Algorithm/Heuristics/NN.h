#ifndef NN_H
#define NN_H

#include <stdio.h>
#include <stdlib.h>

#include "../../algoSelector.h"
#include "../../tsp.h"

/**
 * @brief Applies the nearest neighbor algorithm for every possible starting node.
 * 
 * @param inst Pointer to the instance structure.
 * 
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int NNFromEachNode(instance* inst);

int WeightedNNFromEachNode(instance* inst, double* weights, int* result, double* cost, bool updateInst);

/**
 * @brief Applies the nearest neighbor algorithm to find the solution for the given instance.
 * 
 * @param inst Pointer to the instance structure.
 * @param firstNode The starting node for the algorithm.
 * @param result Pointer to the result vector.
 * @param cost Cost of the solution founded.
 * 
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int weightedNearestNeighbor(instance* inst, int firstNode, int* result, double* cost, double* weights);

int nearestNeighbor(instance* inst, int firstNode, int* result, double* cost);

#endif /* NN_H */