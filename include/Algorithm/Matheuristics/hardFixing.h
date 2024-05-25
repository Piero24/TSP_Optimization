#ifndef HARDFIXING_H
#define HARDFIXING_H

#include "tsp.h"
#include "parser.h"
#include "algoSelector.h"
#include "Algorithm/Heuristics/NN.h"
#include "Algorithm/Exacts/cplexAlg.h"

#include <cplex.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int diving(instance* inst);
void cpxToResult(instance* inst, double* x, int* result);

#endif /* HARDFIXING_H */