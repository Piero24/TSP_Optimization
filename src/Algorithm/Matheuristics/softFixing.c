#include "Algorithm/Matheuristics/softFixing.h"

int localBranching(instance* inst)
{
    verbose_print(inst, 60, "[Local Branching] Initializing algorithm...\n");

	int k = 20; // 20, 30, 40, 50

    // open CPLEX model
	int error, ncomp;
	CPXENVptr env = CPXopenCPLEX(&error);
	assert(error == 0);

	CPXLPptr lp = CPXcreateprob(env, &error, "TSP");
	assert(error == 0);

	build_model(inst, env, lp);

	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
	inst->ncols = ncols;

	// add mipstart
	double *xstar = addMipstart(inst, env, lp);
	double objval = 0;

	for(int a = 0; a < inst->nnodes; a++)
		for(int b = a+1; b<inst->nnodes; b++)
			if(xstar[xpos(a, b, inst)] > 0.5)
				objval += dist(inst, a, b);
	
	int* result = (int*)calloc(inst->nnodes, sizeof(int));
	cpxToResult(inst, xstar, result);

	bestSolution(result, objval, inst);

	int* succ = (int *) calloc(inst->nnodes, sizeof(int));
	int* comp = (int *) calloc(inst->nnodes, sizeof(int));
	int* dim = (int *) calloc(inst->nnodes + 1, sizeof(int));
	
	inst->callback_base = true;
	inst->callback_relax = true;
	inst->posting_base = true;
	inst->posting_relax = false;

	char **cname = (char **) calloc(1, sizeof(char *));
	double rhs = inst->nnodes - k; // n-k
	char sense = 'G';
	cname[0] = (char *) calloc(100, sizeof(char));
	int izero = 0;
	sprintf(cname[0], "localBrancing"); 

	// Set the coordinates of the new solution (how to use it?)
	double *value = (double *) malloc(inst->nnodes * sizeof(double));
	int *index = (int *) malloc(inst->nnodes * sizeof(int));

	verbose_print(inst, 80, "[Local Branching] Initializing done, mipstart cost: %f\n", objval);

	//first check of time
	void* current_time = currentTime();
    struct timespec end = *((struct timespec*)current_time);
	free(current_time);
	
    double time = timeElapsed(&(inst->tstart), &(end));
	
	while(time < inst->time_limit) {
		// add contraint: fix n-k edges
		int nnz = 0;
		
		for ( int h = 0; h < inst->nnodes; h++ )
		{
			for ( int i = h+1; i < inst->nnodes; i++ )
			{
				if ( xstar[xpos(h,i,inst)] < 0.5 ) continue;

				index[nnz] = xpos(i,h, inst);
				value[nnz] = 1.0;
				nnz++;
			}
		}

		error = CPXaddrows(env, lp, 0, 1, nnz, &rhs, &sense, &izero, index, value, NULL, &cname[0]);
		assert(error == 0); // error on CPXaddrows

		// call B&B blackbox
		double new_objval;
		if(time < inst->time_limit) {
			error = branchBound(env, lp, inst, inst->time_limit - time, xstar, &new_objval);
		}else {
			error = 2;
			break;
		}

		if (new_objval < objval)
		{
			objval = new_objval;
			for (int i = 0; i < ncols; i++)
				xstar[i] = xstar[i];
			
			cpxToResult(inst, xstar, result);

			bestSolution(result, objval, inst);

			// add new best solution to cplex as mipstart
			int error2 = addCPLEXMipStart(inst, env, lp, xstar);
			assert(error2 == 0);
		}

		// if cplex exited with an error or by time limit, end the algorithm
		if(error != 0) break;

		// increase number of free edges
		k = k + 10;

		// remove old constraint
		int rowindex = -1;
		error = CPXgetrowindex(env, lp, "localBrancing", &rowindex);
		assert(error == 0);

		error = CPXdelrows(env, lp, rowindex, rowindex);
		assert(error == 0);

		//check time
		current_time = currentTime();
    	end = *((struct timespec*)current_time);
		free(current_time);
	
    	time = timeElapsed(&(inst->tstart), &(end));

	}

	printf("[localBranching] CPLEX status: %d\n", error);

	assert(error == 0 || error == 2);

	// compute connected components and plot
	if(inst->show_gnuplot > -1){
		if(inst->show_gnuplot > 0)
			sleep_ms(inst->show_gnuplot*1000);
		
		build_sol(xstar, inst, succ, comp, dim, &ncomp);
		int** result1 = convertSolution(succ, comp, ncomp, inst);
		show_solution_comps(inst, true, result1, ncomp);

		for(int i=1; i<ncomp+1; i++) free(result1[i]);
		free(result1);
	}
	
	// free and close cplex model   
	free(cname[0]);
	free(cname);
	free(index);
	free(value);

    free(xstar);
	free(result);
	free(succ);
	free(comp);
    free(dim);
	CPXfreeprob(env, &lp);
	CPXcloseCPLEX(&env); 

    return 0;
}