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
 * @brief Calculates the elapsed time between two time points.
 * 
 * @param start Pointer to the start time. On Windows, this is a pointer to `clock_t`.
 * On Unix-like systems, this is a pointer to `struct timespec`.
 * @param end Pointer to the end time. On Windows, this is a pointer to `clock_t`.
 * On Unix-like systems, this is a pointer to `struct timespec`.
 * 
 * @return The elapsed time in seconds as a `double` value.
 */
double timeElapsed(void* start, void* end);

/**
 * @brief Gets the current time.
 * 
 * @return A pointer to the current time. On Windows, this is a pointer to `clock_t`.
 * On Unix-like systems, this is a pointer to `struct timespec`.
 */
void* currentTime();

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

#endif /* ALGOSELECTOR_H */