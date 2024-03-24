#ifndef OPTIMIZATIONS_H
#define OPTIMIZATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#include "NN.h"
#include "../algoSelector.h"
#include "../tsp.h"

/**
 * @brief Reverses the elements in a sub-array defined by the pointers from and to.
 * 
 * This function reverses the elements in the sub-array defined by the pointers from and to.
 * It swaps the elements symmetrically from the outer ends towards the center.
 * 
 * @param from Pointer to the first element of the sub-array.
 * @param to Pointer to the last element of the sub-array.
 */
void reverseSubvector(int* from, int* to);

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
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int twoOpt(instance* inst);

/**
 * @brief Checks if a given node exists in a list.
 * 
 * This function iterates through the elements of the list to determine if 
 * the specified node exists in the list.
 * 
 * @param node Pointer to the node to be checked.
 * @param list Pointer to the list of nodes.
 * @param listLength Length of the list.
 * @return Returns true if the node exists in the list, otherwise false.
 */
bool nodeInList(int* node, int* list, int listLength);

/**
 * @brief Copies the contents of one array to another.
 * 
 * This function copies the contents of the 'from' array to the 'to' array.
 * Memory for the 'to' array is allocated dynamically, and the 
 * caller is responsible for freeing it.
 * 
 * @param from Pointer to the source array.
 * @param to Pointer to the destination array. This pointer will be updated to 
 * point to the newly allocated array.
 * @param length Number of elements to copy.
 */
void copyArray(int* from, int* to, int length);

/**
 * @brief Optimizes a solution by searching in the neighborhood of solutions (in terms of
 *  different edges). Uses tabu search to overcome local minimums
 * 
 * @param result Pointer to the array with the inizial solution that will be optimized. 
 *  Will contain the optimized solution after the function ends.
 * @param cost Pointer to the variable to store the cost of the solution.
 * @param inst Pointer to the instance structure.
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int tabuSearch(instance* inst);

/**
 * @brief 2opt moves with kicks when the best solution gets stuck in local minimum
 * 
 * @param inst Pointer to the instance structure.
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int variableNeighborhoodSearch(instance* inst);

#endif /* OPTIMIZATIONS_H */