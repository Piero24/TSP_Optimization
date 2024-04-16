#ifndef CPLEXALG_H
#define CPLEXALG_H

#include "tsp.h"
#include "parser.h"
#include "algoSelector.h"
#include <cplex.h>

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
 * @brief Converts the solution obtained from build_sol into a list of feasible TSP paths.
 * 
 * @param 
 */
int** convertSolution(int *succ, int *comp, int ncomp, instance* inst);

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

/**
 * @brief
 * 
 * @param xstar
 * @param inst
 * @param succ
 * @param comp
 * @param ncomp
 */
void build_sol(const double *xstar, instance *inst, int *succ, int *comp, int *ncomp);

/**
 * @brief
 * 
 * @param inst
 * @param env
 * @param lp
 * @param ncomp
 * @param comp
 */
void add_SEC(instance* inst, CPXENVptr env, CPXLPptr lp, int ncomp, int* comp);

void mergeComponents(instance* inst, int* ncomp, int* comp, int *succ, double *cost);

int bendersLoop(instance *inst);

#endif /* CPLEXALG_H */