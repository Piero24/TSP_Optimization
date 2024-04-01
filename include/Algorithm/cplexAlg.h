#ifndef CPLEXALG_H
#define CPLEXALG_H

#include "tsp.h"
#include "parser.h"
#include "algoSelector.h"
#include <cplex.h>


/**
 * @brief 
 * 
 * @param 
 * @param 
 * @param 
 * 
 * @return
 */
double dist(int i, int j, instance *inst);

/**
 * @brief 
 * 
 * @param 
 */
int TSPopt(instance *inst);

/**
 * @brief 
 * 
 * @param 
 * @param 
 */
void convertSolution(double *xstar, int* result, instance* inst);

/**
 * @brief 
 * 
 * @param 
 * @param 
 * @param 
 * 
 * @return
 */
int xpos(int i, int j, instance *inst);

/**
 * @brief 
 * 
 * @param 
 * @param 
 * @param 
 */
void build_model(instance *inst, CPXENVptr env, CPXLPptr lp);

#endif /* CPLEXALG_H */