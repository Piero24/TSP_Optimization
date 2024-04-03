#ifndef CPLEXALG_H
#define CPLEXALG_H

#include "tsp.h"
#include "parser.h"
#include "algoSelector.h"
#include <cplex.h>


/**
 * @brief Calculates the distance between two points in a given instance.
 * 
 * @param i The index of the first point.
 * @param j The index of the second point.
 * @param inst Pointer to the instance containing the points and their distances.
 * 
 * @return The distance between points i and j in the instance.
 */
double dist(int i, int j, instance *inst);

/**
 * @brief Solves the Traveling Salesman Problem (TSP) using CPLEX optimization.
 * 
 * @param inst Pointer to the instance containing problem data.
 * 
 * @return Returns 0 upon successful completion, or an appropriate nonzero 
 *  error code if an error occurs.
 */
int TSPopt(instance *inst);

/**
 * @brief Converts the solution obtained from CPLEX into a feasible TSP path.
 * 
 * @param xstar Array containing the solution values obtained from CPLEX.
 * @param result Array to store the converted solution (TSP path).
 * @param inst Pointer to the instance containing problem data.
 */
void convertSolution(double *xstar, int* result, instance* inst);

/**
 * @brief Calculates the position of the (i, j) element in a one-dimensional 
 * array representing a symmetric matrix.
 * 
 * @param i Row index of the element.
 * @param j Column index of the element.
 * @param inst Pointer to the instance containing problem data.
 * 
 * @return The position of the (i, j) element in the one-dimensional 
 *  array representing the symmetric matrix.
 */
int xpos(int i, int j, instance *inst);

/**
 * @brief Builds the optimization model for the Traveling 
 *  Salesman Problem (TSP) using CPLEX.
 * 
 * @param inst Pointer to the instance containing problem data.
 * @param env CPLEX environment pointer.
 * @param lp CPLEX problem pointer.
 */
void build_model(instance *inst, CPXENVptr env, CPXLPptr lp);

#endif /* CPLEXALG_H */