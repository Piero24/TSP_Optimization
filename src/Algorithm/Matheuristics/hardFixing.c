#include "Algorithm/Matheuristics/hardFixing.h"

void cpxToResult(instance* inst, double* x, int* result){
	int* succ = (int *) calloc(inst->nnodes, sizeof(int));
	int* comp = (int *) calloc(inst->nnodes, sizeof(int));
	int* dim = (int *) calloc(inst->nnodes + 1, sizeof(int));
	int ncomp;
	build_sol(x, inst, succ, comp, dim, &ncomp);
 
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
    verbose_print(inst, 80, "[Diving] Initializing algorithm...\n");

    int error;
	CPXENVptr env = CPXopenCPLEX(&error);
	assert(error == 0);

	CPXLPptr lp = CPXcreateprob(env, &error, "TSP");
	assert(error == 0);

	// build TSP problem
	build_model(inst, env, lp);
	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
	inst->ncols = ncols;
	inst->callback_base = true;
	inst->callback_relax = true;
	inst->posting_base = true;
	inst->posting_relax = false;
    
	double *xstar = addMipstart(inst, env, lp);
	double objval = 0.0;

	int counter = 0;
	for(int a = 0; a < inst->nnodes; a++)
		for(int b = a+1; b<inst->nnodes; b++)
			if(xstar[xpos(a, b, inst)] > 0.5){
				objval += dist(inst, a, b);
				counter ++;
			}
	
	int* result = (int *) calloc(inst->nnodes, sizeof(int));
	cpxToResult(inst, xstar, result);
	bestSolution(result, objval, inst);

	double *value = (double *) calloc(ncols, sizeof(double));
	int *index = (int *) calloc(ncols, sizeof(int));
	char **cname = (char **) malloc(sizeof(char *));
	cname[0] = (char *) malloc(100 * sizeof(char));
	double cpx_objval = 0;
	double rhs = 1.0;
	char sense = 'E';
	int izero = 0;
	for(int i=0;i<inst->ncols;i++) value[i] = 1.0;
	
	verbose_print(inst, 80, "[Diving] Initializing done, mipstart cost: %f, counter: %d\n", objval, counter);

	//first check of time
	clock_t end = clock();
    double time;// = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;
	
	while(time < inst->time_limit) {
		double freeEdgesProb = 0.1;

		// fix edges
		for(int a = 0; a < inst->nnodes; a++)
		{
			for(int b = a+1; b<inst->nnodes; b++)
			{
				if(xstar[xpos(a, b, inst)] < 0.5) continue;

				double rand = randomDouble(0, 1);
			
				if(rand < freeEdgesProb){
					sprintf(cname[0], "diving(%d, %d)", a, b); 
					index[0] = xpos(a, b, inst);

					int error = CPXaddrows(env, lp, 0, 1, 1, &rhs, &sense, &izero, index, value, NULL, &cname[0]);
					assert(error == 0); // wrong CPXaddrows
				}
			}
		}

		verbose_print(inst, 90, "[Diving] Constraints added\n");

		// check time
		end = clock();
        //time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;

		// call branch and bound blackbox 
		if(time < inst->time_limit)
			error = branchBound(env, lp, inst, inst->time_limit - time, xstar, &cpx_objval);
		else {
			error = 2;
			break;
		}
		
		verbose_print(inst, 90, "[Diving] branchBound called\n");

		// update free edges probability
		double eps = 0.1 * objval;
		if(objval - cpx_objval < eps && freeEdgesProb < 0.5){
			freeEdgesProb += 0.1;
		}

		// update best solution
		if(cpx_objval < objval){
			objval = cpx_objval;

			cpxToResult(inst, xstar, result);
			bestSolution(result, objval, inst);

			// add new best solution to cplex as mipstart
			int error2 = addCPLEXMipStart(inst, env, lp, xstar);
			assert(error2 == 0);
		}

		verbose_print(inst, 90, "[Diving] data updated\n");

		// if cplex exited with an error or by time limit, end the algorithm
		if(error != 0) break;

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

		verbose_print(inst, 90, "[Diving] constrains removed\n");

		//check time
		end = clock();
        //time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;

	}
	
	assert(error == 0 || error == 2);

	verbose_print(inst, 80, "[Diving] time ended\n");

	// plot
	if(inst->show_gnuplot > -1){
		if(inst->show_gnuplot > 0)
			sleep_ms(inst->show_gnuplot*1000);
		
		int* succ = (int *) calloc(inst->nnodes, sizeof(int));
		int* comp = (int *) calloc(inst->nnodes, sizeof(int));
		int* dim = (int *) calloc(inst->nnodes + 1, sizeof(int));
		int ncomp;
		build_sol(xstar, inst, succ, comp, dim, &ncomp);

		int** result1 = convertSolution(succ, comp, ncomp, inst);
		show_solution_comps(inst, true, result1, ncomp);

		for(int i=1; i<ncomp+1; i++) free(result1[i]);
		free(result1);
		free(dim);
		free(comp);
		free(succ);
	}
	
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
