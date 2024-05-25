#include "Algorithm/Matheuristics/hardFixing.h"

void cpxToResult(instance* inst, double* x, int* result){
	int* succ = (int *) calloc(inst->nnodes, sizeof(int));
	int* comp = (int *) calloc(inst->nnodes, sizeof(int));
	int* dim = (int *) calloc(inst->nnodes + 1, sizeof(int));
	int ncomp;
	build_sol(x, inst, succ, comp, dim, &ncomp);
	
	result = (int *) calloc(inst->nnodes, sizeof(int));
 
	int pos = 0;
	int next = 0;
	do{
		result[pos++] = next;
		next = succ[next];
	}while(next != 0);

	free(dim);
	free(comp);
	free(succ);
}

int diving(instance* inst)
{
    verbose_print(inst, 60, "[Local Branching] Initializing algorithm...\n");

    int error;
	CPXENVptr env = CPXopenCPLEX(&error);
	assert(error == 0);

	CPXLPptr lp = CPXcreateprob(env, &error, "TSP");
	assert(error == 0);

	// build TSP problem
	build_model(inst, env, lp);
	CPXsetdblparam(env, CPXPARAM_TimeLimit, inst->time_limit);
	// CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_ON);
	CPXsetintparam(env, CPX_PARAM_THREADS, 1);
	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
	inst->ncols = ncols;
	inst->callback_base = true;
	inst->callback_relax = true;
	inst->posting_base = false;
	inst->posting_relax = false;
	
	// setting callbacks
	CPXLONG contextid = CPX_CALLBACKCONTEXT_CANDIDATE | CPX_CALLBACKCONTEXT_RELAXATION;
	error = CPXcallbacksetfunc(env, lp, contextid, callbackHandler, inst);
	assert(error == 0);
    
	double *xheu = addMipstart(inst, env, lp);
	double objval;

	for(int a = 0; a < inst->nnodes; a++)
		for(int b = a+1; b<inst->nnodes; b++)
			if(xheu[xpos(a, b, inst)] > 0.5)
				objval += dist(inst, a, b);
	
	int* result;
	cpxToResult(inst, xheu, result);
	bestSolution(result, objval, inst);

	double *xstar = (double *) calloc(ncols, sizeof(double));
	double *value = (double *) calloc(inst->ncols, sizeof(double));
	int *index = (int *) calloc(inst->ncols, sizeof(int));
	char **cname = (char **) malloc(sizeof(char *));
	cname[0] = (char *) malloc(100 * sizeof(char));
	double cpx_objval = 0;
	double rhs = 1.0;
	char sense = 'G';
	int izero = 0;
	for(int i=0;i<inst->ncols;i++) value[i] = 1.0;
	
	//first check of time
	clock_t end = clock();
    double time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;
	
	do {
		//set time limit
		CPXsetdblparam(env, CPXPARAM_TimeLimit, inst->time_limit - time);
		double freeEdgesProb = 0.1;

		// fix edges
		int nnz = 0;
		for(int a = 0; a < inst->nnodes; a++)
		{
			for(int b = a+1; b<inst->nnodes; b++)
			{
				if(xheu[xpos(a, b, inst)] < 0.5) continue;
			
				if(randomDouble(0, 1) < freeEdgesProb){
					sprintf(cname[0], "diving(%d, %d)", a, b); 
					index[nnz] = xpos(a, b, inst);
					nnz++;

					int error = CPXaddrows(env, lp, 0, 1, nnz, &rhs, &sense, &izero, index, value, NULL, &cname[0]);
					assert(error == 0); // wrong CPXaddrows
				}
			}
		} 

		// compute CPLEX solution
		error = CPXmipopt(env,lp);
		assert(error == 0);

		// get CPLEX solution
		CPXgetbestobjval(env, lp, &cpx_objval);
		CPXgetx(env, lp, xstar, 0, ncols - 1);

		// update free edges probability
		double eps = 0.1 * objval;
		if(objval - cpx_objval < eps && freeEdgesProb < 0.5){
			freeEdgesProb += 0.1;
		}

		// update best solution
		if(cpx_objval < objval){
			objval = cpx_objval;

			free(result);
			cpxToResult(inst, xheu, result);

			bestSolution(result, objval, inst);
		}

		// remove fixed edges
		for(int a = 0; a < inst->ncols; a++)
		{
			for(int b = a+1; b<inst->nnodes; b++)
			{
				int rowindex = -1;
				sprintf(cname[0], "diving(%d, %d)", a, b);
				error = CPXgetrowindex (env, lp, cname[0], &rowindex);

				if(error == 0){
					error = CPXdelrows(env, lp, rowindex, rowindex);
					assert(error == 0);
				}
			}
		} 

		//check time
		end = clock();
        time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;

	} while(time < inst->time_limit);

	free(result);
    free(cname[0]);
	free(cname);
	free(index);
	free(value);
	free(xstar);

	CPXfreeprob(env, &lp);
	CPXcloseCPLEX(&env); 

    return 0;
}
