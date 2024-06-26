#ifndef TWOOPT_H
#define TWOOPT_H

#include "optUtils.h"
#include "../../tsp.h"

/**
 * @brief The actual loop of a twoOpt optimization.
 * 
 * @param inst Pointer to the instance structure.
 * @param result Pointer to the array with the inizial solution that will be optimized. 
 *  Will contain the optimized solution after the function ends.
 * @param cost Pointer to the variable to store the cost of the solution.
 * 
 * @param costs Pointer to array of costs function (needed for plotting).
 * @param nCosts Number of elements of costs.
 * @param xIndex Iterations counter (needed for plotting).
 * 
 * @param VNS True if the caller function is VNS, false otherwise.
 * @param plotFlag True if the costs function should be plotted, false otherwise.
 * @param mipstart True if the caller function is mipstart, false otherwise.
 * 
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int twoOptLoop(instance* inst, int* result, double* cost, point* costs, int* nCosts, int* xIndex, bool VNS, bool plotFlag, bool mipstart);

/**
 * @brief Optimizes a solution by removing intersections
 * * Check every quartet of nodes A, B, C and D where D is the predecessor of A,
 * * C is the successor of B and there is a path starting from A to B that does not 
 * * go through C and D.
 *  
 * * visualization:   | - | - | D | A | - | - | - | B | C | - | - | - |
 * *         path between A and B-->  ^^^^^^^^^^^^^
 * 
 * * We try to see if swapping A-D with A-C and B-C with B-D would reduce the cost.
 * * We can achieve this by using the formula for the euclidean distance d().
 * * By applying it to the pats d(A,C) + d(B,D) < d(A,D) + d(B,C) we have that if
 * * d(A,C) + d(B,D) is less then d(A,D) + d(B,C) the new path is better
 * * then the old one and we have to swap all the inner node (sub-array from A to B).
 * 
 * * from:            | - | - | D | A | x | y | z | B | C | - | - | - |
 * * to:              | - | - | D | B | z | y | x | A | C | - | - | - |
 * 
 * * When there is no more swap to do the new path found is the final path we must return.
 * 
 * @param result Pointer to the array with the inizial solution that will be optimized. 
 *  Will contain the optimized solution after the function ends.
 * @param cost Pointer to the variable to store the cost of the solution.
 * @param inst Pointer to the instance structure.
 * 
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int twoOpt(instance* inst);

#endif /* TWOOPT_H */