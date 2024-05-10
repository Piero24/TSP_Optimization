#ifndef OPTUTILS_H
#define OPTUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#include "../Heuristics/NN.h"
#include "../../algoSelector.h"
#include "../../tsp.h"

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
 * @brief Checks if a given node exists in a list.
 * 
 * This function iterates through the elements of the list to determine if 
 * the specified node exists in the list.
 * 
 * @param node Pointer to the node to be checked.
 * @param list Pointer to the list of nodes.
 * @param listLength Length of the list.
 * 
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

void kick(double* cost, int* result, instance* inst);

#endif /* OPTUTILS_H */