#ifndef ALGOSELECTOR_H
#define ALGOSELECTOR_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "Algorithm/NN.h"
#include "Algorithm/cplexAlg.h"
#include "Algorithm/optimizations.h"
#include "parser.h"
#include "tsp.h"

/**
 * @brief Measure the time it takes to run the selected algorithm and print 
 * the results to the standard output.
 * 
 * @param inst Pointer to the instance structure.
 * @return 0 if the algorithm runs successfully.
 */
int apply_algorithm(instance* inst);

/**
 * @brief Selects and applies the appropriate algorithm based on the model type 
 * specified in the instance.
 * 
 * @param inst Pointer to the instance structure.
 * @param firstNode The starting node for the algorithm.
 * @return 0 if the algorithm runs successfully.
 */
int algorithmSelector(instance* inst);

/**
 * @brief Substitute the path of the old best solution with the path of 
 * new solution.
 * 
 * @param result Pointer to the array that store the best solution.
 * @param cost Cost of the best solution.
 * @param inst Pointer to the instance structure that contain the information
 * of the new best solution.
 * @return 0 if the time limit isn't violated yet, 1 otherwise
 */
int bestSolution(int* result, double cost, instance* inst);

#endif /* ALGOSELECTOR_H */