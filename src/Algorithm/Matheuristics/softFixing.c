#include "Algorithm/Matheuristics/softFixing.h"

int localBranching(instance* inst, CPXENVptr env, CPXLPptr lp)
{
    verbose_print(inst, 60, "[Local Branching] Initializing algorithm...\n");
    
	addMipstart(inst, env, lp);

    int k = 20; // 20, 30, 40, 50
    int* max_limit = calloc(inst->nnodes, sizeof( int ));
    double new_sol = 0.0;

    // Set the coordinates of the new solution (how to use it?)
    // setConstraint(inst, int* index, double* value, int* comp, int k, double* rhs, int* nnz);

    // Branch and Cut
    // Which is the func to start the branch and cut?

    
    return 0;
}