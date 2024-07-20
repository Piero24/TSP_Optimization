#ifndef ALGOSELECTOR_H
#define ALGOSELECTOR_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "Algorithm/Heuristics/NN.h"
#include "Algorithm/Exacts/cplexAlg.h"
#include "Algorithm/Optimizators/twoOpt.h"
#include "Algorithm/Optimizators/tabuSearch.h"
#include "Algorithm/Optimizators/vns.h"
#include "Algorithm/Matheuristics/hardFixing.h"
#include "Algorithm/Matheuristics/softFixing.h"
#include "parser.h"
#include "tsp.h"

/**
 * @brief Measure the time it takes to run the selected algorithm and print 
 * the results to the standard output.
 * 
 * @param inst Pointer to the instance structure.
 * 
 * @return 0 if the algorithm runs successfully.
 */
int apply_algorithm(instance* inst);

/**
 * @brief Selects and applies the appropriate algorithm based on the model type 
 * specified in the instance.
 * 
 * @param inst Pointer to the instance structure.
 * @param firstNode The starting node for the algorithm.
 * 
 * @return 0 if the algorithm runs successfully.
 */
int algorithmSelector(instance* inst);

/**
 * @brief Randomly generates a solution for the TSP instance.
 * 
 * @param inst Pointer to the instance structure.
 */
void sleep_ms(int milliseconds);

/**
 * @brief Substitute the path of the old best solution with the path of 
 * new solution.
 * 
 * @param result Pointer to the array that store the best solution.
 * @param cost Cost of the best solution.
 * @param inst Pointer to the instance structure that contain the information
 * of the new best solution.
 * 
 * @return 0 if the time limit isn't violated yet, 1 otherwise
 */
int bestSolution(int* result, double cost, instance* inst);



double timeElapsed(void* start, void* end);

void* currentTime();

#endif /* ALGOSELECTOR_H */