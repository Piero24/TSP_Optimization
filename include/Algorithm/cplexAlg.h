#ifndef CPLEXALG_H
#define CPLEXALG_H

#include "tsp.h"
#include "parser.h"
#include "algoSelector.h"

#include <cplex.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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
 * @brief Converts the solution representation from succ and comp arrays to a 2D array.
 *  This function converts the solution representation from successor and 
 *  component arrays to a 2D array, where each row represents a connected 
 *  component of the solution.
 * 
 * @param succ Pointer to the successor array.
 * @param comp Pointer to the component array.
 * @param ncomp The number of components in the solution.
 * @param inst Pointer to the instance structure.
 * 
 * @return Returns a 2D array representing the solution.
 */
int** convertSolution(int *succ, int *comp, int ncomp, instance* inst);

/**
 * @brief Calculates the position of the (i, j) element in a one-dimensional 
 *  array representing a symmetric matrix.
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

/**
 * @brief Builds successor and component arrays based on the solution vector.
 * This function constructs the successor and component arrays based on the provided
 * solution vector `xstar`, which represents the selected edges in the graph. It traverses
 * the graph to identify connected components and assigns successor and component values
 * accordingly.
 * 
 * @param xstar The solution vector representing selected edges.
 * @param inst Pointer to the instance structure.
 * @param succ Pointer to the successor array to be populated.
 * @param comp Pointer to the component array to be populated.
 * @param ncomp Pointer to store the total number of components found.
 */
void build_sol(const double *xstar, instance *inst, int *succ, int *comp, int *ncomp);

/**
 * @brief Adds subtour elimination constraints (SECs) to the LP model.
 * This function adds subtour elimination constraints (SECs) to the linear programming (LP) model
 * represented by the provided CPLEX environment `env` and LP problem `lp`. The SECs are constructed
 * based on the connected components identified in the graph represented by the `comp` array.
 * 
 * @param inst Pointer to the instance structure.
 * @param env Pointer to the CPLEX environment.
 * @param lp Pointer to the CPLEX LP problem.
 * @param ncomp The total number of connected components.
 * @param comp Array containing the component assignments for each node.
 */
void add_SEC(instance* inst, CPXENVptr env, CPXLPptr lp, int ncomp, int* comp);

/**
 * @brief Merges connected components based on the best swap of nodes.
 * This function merges connected components in the graph based on the best swap of nodes
 * to minimize the total cost. It iterates through pairs of nodes from different components,
 * calculates the cost of swapping their successors, and selects the swap that results in the
 * minimum increase in cost. The function updates the successor array `succ`, component array `comp`,
 * and total cost accordingly.
 * 
 * @param inst Pointer to the instance structure.
 * @param ncomp Pointer to the total number of components.
 * @param comp Array containing the component assignments for each node.
 * @param succ Array containing the successor of each node.
 * @param cost Pointer to the total cost, which is updated after each merge.
 */
void mergeComponents(instance* inst, int* ncomp, int* comp, int *succ, double *cost);

/**
 * @brief Executes the Benders' decomposition algorithm for solving the TSP.
 * This function implements the Benders' decomposition algorithm to solve the Traveling Salesman Problem (TSP).
 * It iteratively solves the TSP using CPLEX, computes the connected components of the solution,
 * adds sub-tour elimination constraints, and merges components until the time limit is reached.
 * 
 * @param inst Pointer to the instance structure.
 * @param gluing Says whether or not the function should do gluing at each iteration.
 * 
 * @return Returns 0 if the algorithm completes successfully, or an appropriate nonzero error code.
 */
int bendersLoop(instance *inst, bool gluing);

/**
 * @brief Performs the 2-opt optimization on the given solution.
 * This function applies the 2-opt optimization to the provided solution, aiming to improve its quality.
 * 
 * @param inst Pointer to the instance structure.
 * @param result Pointer to the array representing the solution.
 * @param cost The cost of the provided solution.
 * 
 * @return Returns 0 if the optimization completes successfully, or an appropriate nonzero error code.
 */
int gluing2Opt(instance* inst, int* result, double cost);

#endif /* CPLEXALG_H */