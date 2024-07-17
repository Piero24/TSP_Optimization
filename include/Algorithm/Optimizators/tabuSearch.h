#ifndef TABUSEARCH_H
#define TABUSEARCH_H

#include "optUtils.h"
#include "../../tsp.h"

/**
 * @brief Optimizes a solution by searching in the neighborhood of solutions (in terms of
 *  different edges). Uses tabu search to overcome local minimums
 * 
 * @param result Pointer to the array with the initial solution that will be optimized. 
 *  Will contain the optimized solution after the function ends.
 * @param cost Pointer to the variable to store the cost of the solution.
 * @param inst Pointer to the instance structure.
 * 
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int tabuSearch(instance* inst);

#endif /* TABUSEARCH_H */