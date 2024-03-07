#ifndef ALGOSELECTOR_H
#define ALGOSELECTOR_H

#include "../include/tsp.h"

/**
 * @brief Measure the time it takes to run the selected algorithm and print 
 * the results to the standard output.
 * 
 * @param inst Pointer to the instance structure.
 * @param bool Boolean true if you want to print only the best 
 * solution false all the possible solutions.
 * @return 0 if the algorithm runs successfully.
 */
int apply_algorithm(instance* inst, bool onlyBestSolution);

/**
 * @brief Selects and applies the appropriate algorithm based on the model type 
 * specified in the instance.
 * 
 * @param result Pointer to the array to store the resulting solution.
 * @param cost Pointer to the variable to store the cost of the solution.
 * @param inst Pointer to the instance structure.
 * @param firstNode The starting node for the algorithm.
 * @return 0 if the algorithm runs successfully.
 */
int algorithmSelector(int* result, double* cost, instance* inst, int firstNode);

#endif /* ALGOSELECTOR_H */