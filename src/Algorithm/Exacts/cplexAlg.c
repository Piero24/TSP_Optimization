#include "Algorithm/Exacts/cplexAlg.h"
#include "mincut.h"

int branchBound(CPXENVptr env, CPXLPptr lp, instance* inst, double time_limit, double* xstar, double* objval)
{
	verbose_print(inst, 90, "[CPLEX] Initializing algorithm...\n");

	// CPXsetintparam(env, CPXPARAM_ScreenOutput, CPX_ON);
	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
	inst->ncols = ncols;
	
	// setting callbacks
	CPXLONG contextid = CPX_CALLBACKCONTEXT_CANDIDATE | CPX_CALLBACKCONTEXT_RELAXATION;
	int error = CPXcallbacksetfunc(env, lp, contextid, callbackHandler, inst);
	assert(error == 0);

	// add time limit
	CPXsetdblparam(env, CPXPARAM_TimeLimit, time_limit);
	
	// compute CPLEX solution
	verbose_print(inst, 90, "[CPLEX] Getting best solution\n");
	error = CPXmipopt(env, lp);
	assert(error == 0);

	// get CPLEX solution
	CPXgetx(env, lp, xstar, 0, ncols - 1);
    CPXgetobjval(env, lp, objval);

	// check cplex status
	int solstat = CPXgetstat(env, lp);

	switch (solstat) {
		case CPXMIP_OPTIMAL:
			return 0;
		case CPXMIP_OPTIMAL_TOL:
			return 0;
		case CPXMIP_INFEASIBLE:
			return 1;
		case CPXMIP_INForUNBD:
			return 1;
		case CPXMIP_UNBOUNDED:
			return 1;
		case CPXMIP_TIME_LIM_FEAS:
			return 2;
		case CPXMIP_TIME_LIM_INFEAS:
			return 2;
		
		default:
			return -1;
	}
	
	return 0;
}

int TSPopt(instance *inst)
{  
	verbose_print(inst, 60, "[CPLEX] Initializing algorithm...\n");

	// open CPLEX model
	int error;
	CPXENVptr env = CPXopenCPLEX(&error);
	assert(error == 0);

	CPXLPptr lp = CPXcreateprob(env, &error, "TSP");
	assert(error == 0);

	// build TSP problem
	build_model(inst, env, lp);
	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
	inst->ncols = ncols;
	
	// setting callbacks
	CPXLONG contextid = CPX_CALLBACKCONTEXT_CANDIDATE | CPX_CALLBACKCONTEXT_RELAXATION;
	error = CPXcallbacksetfunc(env, lp, contextid, callbackHandler, inst);
	assert(error == 0);

	// add mipstart
	double *xstar;
	if(inst->mipstart)
		xstar = addMipstart(inst, env, lp);
	else
		xstar = (double *) calloc(inst->ncols, sizeof(double)); 
	
	// set CPLEX time limit
	void* current_time = currentTime();
    struct timespec end = *((struct timespec*)current_time);
	free(current_time);
	
    double time = timeElapsed(&(inst->tstart), &(end));
	
	CPXsetdblparam(env, CPXPARAM_TimeLimit, inst->time_limit - time);
	
	// compute CPLEX solution
	verbose_print(inst, 60, "[CPLEX] Getting best solution\n");
	error = CPXmipopt(env,lp);
	assert(error == 0);

	// get CPLEX solution
	double objval = 0;
    CPXgetobjval(env, lp, &objval);
	CPXgetx(env, lp, xstar, 0, ncols - 1);
    
    // print solution
    if(inst->verbose >= 95){
        printf("[CPLEX] Costs:%f\n", objval);

        for ( int i = 0; i < inst->nnodes; i++ )
        {
            for ( int j = i+1; j < inst->nnodes; j++ )
            {
				// print the selected edges
                if ( xstar[xpos(i,j,inst)] > 0.5 ) 
				{
					// xstar it's never 0 o 1 precisely
					printf("[CPLEX] x(%3d,%3d) = 1\n", i+1,j+1);
				}
            }
        }
    }

	//convert solution for plotting
    int** resultPlot;
	int ncomp;
	int* succ = (int *) calloc(inst->nnodes, sizeof(int));
	int* comp = (int *) calloc(inst->nnodes, sizeof(int));
	int* dim = (int *) calloc(inst->nnodes + 1, sizeof(int));

	current_time = currentTime();
	inst->tbest = *((struct timespec*)current_time);
	free(current_time);

	inst->best_lb = objval;

	if(inst->callback_base || inst->callback_relax){
		inst->zbest = objval;
	}
	
	build_sol(xstar, inst, succ, comp, dim, &ncomp);
	resultPlot = convertSolution(succ, comp, ncomp, inst);

	if(inst->show_gnuplot > -1){
        if(inst->show_gnuplot > 0)
            sleep_ms(inst->show_gnuplot*1000);
        show_solution_comps(inst, true, resultPlot, ncomp);
    }

	// free and close cplex model   
    for(int i=1; i<ncomp+1; i++) free(resultPlot[i]);
	free(resultPlot);
    free(xstar);
    free(succ);
    free(dim);
    free(comp);
	CPXfreeprob(env, &lp);
	CPXcloseCPLEX(&env); 

	return 0;
}

double* addMipstart(instance* inst, CPXENVptr env, CPXLPptr lp)
{
	// heuristic mipstart
	double objval = 0;
	int firstNode = randomInt(0, inst->nnodes-1);

	int* result = (int*) calloc(inst->nnodes, sizeof(int));
	nearestNeighbor(inst, firstNode, result, &objval);
	mipstart2Opt(inst, result, &objval);

	if(inst->debug){	
		// check value
		double costTester = 0.0;
		for(int i=0; i<inst->nnodes-1; i++)
			costTester += dist(inst, result[i], result[i+1]);
		costTester += dist(inst, result[0], result[inst->nnodes-1]);

		if(fabs(objval - costTester) > EPS)
			printf("objval: %f\ncostTester: %f\n", objval, costTester);
		assert(fabs(objval - costTester) < EPS);
	}

	// convert heuristic to CPLEX format
	double* xstar = (double *) calloc(inst->ncols, sizeof(double));
	
	for(int i=0; i<inst->ncols; i++)
		xstar[i] = 0.0;
	
	for(int i=0; i<inst->nnodes-1; i++)
		xstar[xpos(result[i], result[i+1], inst)] = 1.0;
	xstar[xpos(result[inst->nnodes-1], result[0], inst)] = 1.0;

	int error = addCPLEXMipStart(inst, env, lp, xstar);
	assert(error == 0);
	
	free(result);
	return xstar;
}

int addCPLEXMipStart(instance* inst, CPXENVptr env, CPXLPptr lp, double* x){
	int *ind = (int *) malloc(inst->ncols * sizeof(int));
	for ( int j = 0; j < inst->ncols; j++ ) ind[j] = j;
	
	int effortlevel = CPX_MIPSTART_NOCHECK;  
	int beg = 0;

	// add solution to cplex as mipstart
	int error = CPXaddmipstarts(env, lp, 1, inst->ncols, &beg, ind, x, &effortlevel, NULL);

	free(ind);

	return error;
}

int bendersLoop(instance *inst, bool gluing)
{  
	verbose_print(inst, 60, "[Benders] Initializing algorithm...\n");

	// open and build CPLEX model
	int error, ncomp;
	CPXENVptr env = CPXopenCPLEX(&error);
	assert(error == 0);

	CPXLPptr lp = CPXcreateprob(env, &error, "TSP"); 
	assert(error == 0);

	build_model(inst, env, lp);

	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
    double objval = 0;
	int* succ = (int *) calloc(inst->nnodes, sizeof(int));
	int* comp = (int *) calloc(inst->nnodes, sizeof(int));
	int* dim = (int *) calloc(inst->nnodes + 1, sizeof(int));
	double *xstar = (double *) calloc(ncols, sizeof(double));

	verbose_print(inst, 60, "[Benders] Algorithm initialized, starting processing...\n");
	
	void* current_time = currentTime();
    struct timespec end = *((struct timespec*)current_time);
	free(current_time);
	
    double time = timeElapsed(&(inst->tstart), &(end));
	
	do {
		//set time limit
		CPXsetdblparam(env, CPXPARAM_TimeLimit, inst->time_limit - time);

		//compute solution
		error = CPXmipopt(env,lp);
		assert(error == 0);

		//get solution
		CPXgetobjval(env, lp, &objval);
		CPXgetx(env, lp, xstar, 0, ncols - 1);

		//save lower bound
		if(inst->best_lb > objval)
			inst->best_lb = objval;
		
		// compute connected components
		build_sol(xstar, inst, succ, comp, dim, &ncomp);

		verbose_print(inst, 80, "[Benders] Found solution with %d components with cost %f\n", ncomp, objval);
		
		// if there is only 1 connected compotent the solution is found
		if(ncomp == 1) break;

		// otherwise add sub-tour elimination contraints
		add_SEC(inst, env, lp, ncomp, comp);

		// merge components
		if(gluing){
			int** result;

			if(inst->show_gnuplot > -1){
				if(inst->show_gnuplot > 0)
					sleep_ms(inst->show_gnuplot*1000);
				
				result = convertSolution(succ, comp, ncomp, inst);
				show_solution_comps(inst, true, result, ncomp);

				for(int i=1; i<ncomp+1; i++) free(result[i]);
				free(result);
			}

			mergeComponents(inst, &ncomp, comp, succ, &objval);
			result = convertSolution(succ, comp, ncomp, inst);

			if(inst->show_gnuplot > -1){
				if(inst->show_gnuplot > 0)
					sleep_ms(inst->show_gnuplot*1000);

				show_solution_comps(inst, true, result, ncomp);
			}
			
			gluing2Opt(inst, result[1], objval);

			if(inst->show_gnuplot > -1){
				if(inst->show_gnuplot > 0)
					sleep_ms(inst->show_gnuplot*1000);

				show_solution_comps(inst, true, result, ncomp);
			}

			verbose_print(inst, 80, "[Benders - Gluing] Merged solution has cost %f\n", objval);
			
			if(objval < inst->zbest){
				bestSolution(result[1], objval, inst);

				// convert heuristic to CPLEX format
				double* x = (double *) calloc(inst->ncols, sizeof(double));
				
				for(int i=0; i<inst->ncols; i++)
					x[i] = 0.0;
				
				for(int i=0; i<inst->nnodes-1; i++)
					x[xpos(result[1][i], result[1][i+1], inst)] = 1.0;
				x[xpos(result[1][inst->nnodes-1], result[1][0], inst)] = 1.0;

				int error = addCPLEXMipStart(inst, env, lp, x);
				assert(error == 0);

				free(x);
				//*/
			}
			
			if(inst->debug){
				for(int i=1; i<inst->nnodes; i++)
					for(int j=i+1; j<inst->nnodes; j++)
						assert(result[1][i] != result[1][j]);
			}

			for(int i=1; i<ncomp+1; i++) free(result[i]);
			free(result);

		} else if(inst->show_gnuplot > -1)
		{
			if(inst->show_gnuplot > 0)
				sleep_ms(inst->show_gnuplot*1000);

			int** result = convertSolution(succ, comp, ncomp, inst);
			show_solution_comps(inst, true, result, ncomp);
		}

		//check time
		current_time = currentTime();
		end = *((struct timespec*)current_time);
		free(current_time);
        
		time = timeElapsed(&(inst->tstart), &(end));

	} while(time < inst->time_limit);
	
    // print solution
    if(inst->verbose >= 100)
	{
        for ( int i = 0; i < inst->nnodes; i++ )
        {
            for ( int j = i+1; j < inst->nnodes; j++ )
            {
				// print the selected edges
                if ( xstar[xpos(i,j,inst)] > 0.5 ) 
				{
					// xstar itìs never 0 o 1 precisely
					printf("x(%3d,%3d) = 1\n", i+1,j+1);
				}
            }
        }
    }
	
	if(ncomp == 1)
	{
		int** result = convertSolution(succ, comp, ncomp, inst);
		
		bestSolution(result[1], objval, inst);

		for(int i=1; i<ncomp+1; i++) free(result[i]);
		free(result);

	}else 
		verbose_print(inst, 60, "[Benders] solution not found");

	// free and close cplex model   
    free(xstar);
	free(succ);
	free(comp);
    free(dim);
	CPXfreeprob(env, &lp);
	CPXcloseCPLEX(&env); 

	return 0; // or an appropriate nonzero error code
}

int** convertSolution(int *succ, int *comp, int ncomp, instance* inst)
{
	verbose_print(inst, 95, "[convertSolution] Creating result array.\n");

	int** result = (int **) calloc(ncomp+1, sizeof(int*));
	for(int i=1; i<ncomp+1; i++)
	{
		result[i] = (int *) calloc(inst->nnodes, sizeof(int));

		for(int j=0; j<inst->nnodes; j++)
		{
			result[i][j] = -1;
		}
	}

	verbose_print(inst, 95, "[convertSolution] Creating indexes array.\n");

	int* indexes = (int *) calloc(ncomp+1, sizeof(int));
	for(int i=0; i<ncomp+1; i++) indexes[i] = 0;

	verbose_print(inst, 95, "[convertSolution] Starting converting. ncomp %d\n", ncomp);

    for(int i=0; i<inst->nnodes; i++)
	{
		if(indexes[comp[i]] == 0)
		{
			result[comp[i]][0] = i;
			indexes[comp[i]]++;
			int next = succ[i];

			while(next != i)
			{
				result[comp[i]][indexes[comp[i]]] = next;
				indexes[comp[i]]++;
				next = succ[next];
			}
		}
	}

	if(inst->verbose >= 100)
		for(int i=1; i<=ncomp; i++)
		{
			printf("[convertSolution]");
			for(int j=0; j<inst->nnodes && result[i][j] != -1; j++)
			{
				printf(" result[%d][%d] %d\t", i, j, result[i][j]);
			}
			printf("\n");
		}


	free(indexes);
	return result;
}

void mergeComponents(instance* inst, int* ncomp, int* comp, int *succ, double *cost)
{
	verbose_print(inst, 91, "[mergeComponents] Starting the merge of %d components\n", *ncomp);
	int count = 0;
	while(*ncomp > 1){
		verbose_print(inst, 95, "[mergeComponents] Current #components: %d\n", *ncomp);

		// find best swap
		int bestA = -1, bestB = -1;
		double bestDiffC = DBL_MAX;
		for(int A=0;A<inst->nnodes-1;A++)
		{
			for(int B=A+1;B<inst->nnodes;B++)
			{
				if(comp[A] == comp[B])
					continue;
				
				int A1 = succ[A], B1 = succ[B];
				double diffC = - dist(inst, A, A1) - dist(inst, B, B1) + dist(inst, A, B1) + dist(inst, B, A1);
				
				if(diffC < bestDiffC)
				{
					bestDiffC = diffC;
					bestA = A;
					bestB = B;
				}
			}
		}

		// do best swap
		int A1 = succ[bestA], B1 = succ[bestB];
		
		verbose_print(inst, 95, "[mergeComponents] Best merging found: A: %d\tA1: %d\tB: %d\tB1: %d\n", bestA, A1, bestB, B1);
		verbose_print(inst, 95, "[mergeComponents] Best merging from A->A1 and B->B1 to A->B1 and B->A1\n");
		verbose_print(inst, 95, "[mergeComponents] Best merging has diffC: %f\n", bestDiffC);

		succ[bestA] = B1;
		succ[bestB] = A1;

		int compMin = -1, compMax = -1;

		if(comp[bestA] < comp[bestB]){
			compMin = comp[bestA];
			compMax = comp[bestB];
		} else {
			compMin = comp[bestB];
			compMax = comp[bestA];
		}

		for(int i=0;i<inst->nnodes;i++){
			if(comp[i] == compMax)
				comp[i] = compMin;
			else if(comp[i] > compMax)
				comp[i]--;
		}
		
		*cost += bestDiffC;
		
		*ncomp = *ncomp - 1;
		count ++;

		verbose_print(inst, 95, "[mergeComponents] Best merging done. ncomp %d\n", *ncomp);
	}
}

int gluing2Opt(instance* inst, int* result, double cost)
{   
    inst->plotCosts = NULL;
    int nCosts = 0, xIndex = 0;

    verbose_print(inst, 90, "[Gluing - 2opt] Starting optimization.\n");

    twoOptLoop(inst, result, &cost, NULL, &nCosts, &xIndex, false, false, false);

    verbose_print(inst, 90, "[Gluing - 2opt] Optimization completed.\n\n");

    bestSolution(result, cost, inst);
    
    return 0;
}

int mipstart2Opt(instance* inst, int* result, double* cost)
{   
    inst->plotCosts = NULL;
    int nCosts = 0, xIndex = 0;

    verbose_print(inst, 90, "[mipstart - 2opt] Starting optimization.\n");

    twoOptLoop(inst, result, cost, NULL, &nCosts, &xIndex, false, false, true);

    verbose_print(inst, 90, "[mipstart - 2opt] Optimization completed.\n\n");
    
    return 0;
}

void add_SEC(instance* inst, CPXENVptr env, CPXLPptr lp, int ncomp, int* comp){
	assert(ncomp > 1);
	
	int izero = 0;
	int ncols = CPXgetnumcols(env, lp);
	int* index = (int*) calloc(ncols, sizeof(int)); // indici delle variabili con coefficiente diverso da zero 
	double* value = (double*) calloc(ncols, sizeof(double)); // valore dei coefficienti della sommatoria
	char **cname = (char **) calloc(1, sizeof(char *));
	cname[0] = (char *) calloc(100, sizeof(char));
	char sense = 'L'; // <=

	for(int k=1; k<=ncomp; k++){
		int nnz = 0; // number of non zero
		double rhs = -1.0; // right hand side of contraint
		sprintf(cname[0], "component(%d)", k+1); 
		
		// aggiungo vincolo per component #k
		setConstraint(inst, index, value, comp, k, &rhs, &nnz);
		
		CPXaddrows(env, lp, 0, 1, nnz, &rhs, &sense, &izero, index, value, NULL, cname);
	}

	free(cname[0]);
	free(cname);
	free(index);
	free(value);
}

int xpos(int i, int j, instance *inst)
{ 
	assert(i != j);

	if ( i > j ) return xpos(j,i,inst);

	int pos = i * inst->nnodes + j - (( i + 1 ) * ( i + 2 )) / 2;
	return pos;
}

void build_model(instance *inst, CPXENVptr env, CPXLPptr lp)
{    
	int izero = 0;
	char binary = 'B'; 
	
	// (char **) required by cplex...
	char **cname = (char **) calloc(1, sizeof(char *));
	cname[0] = (char *) calloc(100, sizeof(char));

	// add binary var.s x(i,j) for i < j  
	for ( int i = 0; i < inst->nnodes; i++ )
	{
		for ( int j = i+1; j < inst->nnodes; j++ )
		{
			// x(1,2), x(1,3) ....
			sprintf(cname[0], "x(%d,%d)", i+1,j+1);
			// cost == distance   
			double obj = dist(inst, i, j);
			double lb = 0.0;
			double ub = 1.0;

			int error = CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname);
			assert(error == 0); // wrong CPXnewcols on x var.s

			error = CPXgetnumcols(env,lp)-1 != xpos(i,j, inst);
			assert(error == 0); // wrong position for x var.s
		}
	} 

	// add degree constr.s 
	int *index = (int *) malloc(inst->nnodes * sizeof(int));

	// coefficients to indicate which sides I'm checking (all those connected to h except h itself)
	double *value = (double *) malloc(inst->nnodes * sizeof(double));
	
	// add the degree constraints
	for ( int h = 0; h < inst->nnodes; h++ )
	{
		double rhs = 2.0;
		// 'E' for equality constraint 
		char sense = 'E';
		sprintf(cname[0], "degree(%d)", h+1); 
		int nnz = 0;
		for ( int i = 0; i < inst->nnodes; i++ )
		{
			if ( i == h ) continue;
			index[nnz] = xpos(i,h, inst);
			value[nnz] = 1.0;
			nnz++;
		}
		
		int error = CPXaddrows(env, lp, 0, 1, nnz, &rhs, &sense, &izero, index, value, NULL, &cname[0]);
		assert(error == 0); // wrong CPXaddrows [degree]
	} 

    free(value);
    free(index);	
	
	if ( inst->verbose >= 100 ) 
	{
		// Create the directory for the model files
		#ifdef _WIN32
			_mkdir("Archive");
			_mkdir("Archive/Cplex");
		#else
			mkdir("Archive", 0777);
			mkdir("Archive/Cplex", 0777);
		#endif
		
		CPXwriteprob(env, lp, "Archiive/Cplex/model.lp", NULL);
	}   

	free(cname[0]);
	free(cname);
}

void build_sol(const double *xstar, instance *inst, int *succ, int *comp, int *dim, int *ncomp) // build succ() and comp() wrt xstar()...
{   
	if(inst->debug){
		int *degree = (int *) calloc(inst->nnodes, sizeof(int));
		for ( int i = 0; i < inst->nnodes-1; i++ )
		{
			for ( int j = i+1; j < inst->nnodes; j++ )
			{
				int k = xpos(i,j,inst);
				if (fabs(xstar[k]) > EPS && fabs(xstar[k]-1.0) > EPS){
					printf("i %d j %d xpos %d\n", i, j, k);
					printf("fabs(xstar[k]) %f, fabs(xstar[k]-1.0) %f\n", fabs(xstar[k]), fabs(xstar[k]-1.0));
					printf("\n\tWrong xstar in build_sol()\n\n");
					exit(0);
				}
				if (xstar[k] > 0.5) 
				{
					++degree[i];
					++degree[j];
				}
			}
		}
		for ( int i = 0; i < inst->nnodes; i++ )
		{
			assert(degree[i] == 2); // wrong degree in build_sol()
		}	
		free(degree);
	}

	*ncomp = 0;
	for ( int i = 0; i < inst->nnodes; i++ )
	{
		succ[i] = -1;
		comp[i] = -1;
		dim[i] = 0;
	}
	dim[inst->nnodes] = 0;
	
	for ( int start = 0; start < inst->nnodes; start++ )
	{
		if ( comp[start] >= 0 ) continue;  // node "start" was already visited, just skip it

		// a new component is found
		(*ncomp)++;
		int i = start;
		int done = 0;
		while ( !done )  // go and visit the current component
		{
			comp[i] = *ncomp;
			dim[*ncomp] += 1;
			done = 1;
			for ( int j = 0; j < inst->nnodes; j++ )
			{
				if ( i != j && xstar[xpos(i,j,inst)] > 0.5 && comp[j] == -1 ) // the edge [i,j] is selected in xstar and j was not visited before 
				{
					succ[i] = j;
					i = j;
					done = 0;
					break;
				}
			}
		}	
		succ[i] = start;  // last arc to close the cycle
		
		// go to the next component...
	}
}

void setConstraint(instance* inst, int* index, double* value, int* comp, int k, double* rhs, int* nnz)
{
	for(int i=0; i<inst->nnodes-1; i++){
		if(comp[i] != k) continue;
		*rhs =  *rhs+1;
		for(int j=i+1; j<inst->nnodes; j++){
			if(comp[j] != k) continue;
			
			index[*nnz] = xpos(i,j, inst);
			value[*nnz] = 1.0;
			*nnz = *nnz+1;
		}
	}
}

static int CPXPUBLIC callbackHandler(CPXCALLBACKCONTEXTptr context, CPXLONG contextid, void *userhandle)
{
	// get instance and cplex solution
	instance* inst = (instance*) userhandle;
	int error = 0;

	// choose action based on context id
	if(contextid == CPX_CALLBACKCONTEXT_CANDIDATE && inst->callback_base)
		candidateCallback(context, inst);

	else if(contextid == CPX_CALLBACKCONTEXT_RELAXATION && inst->callback_relax){
		relaxationCallback(context, inst);
	}

	else
		return 0;

	assert(error == 0);
	return 0;
}

int posting_base(instance* inst, CPXCALLBACKCONTEXTptr context, int* succ, int* comp, int ncomp, double* objval)
{
	// DEBUG PLOTTING (before gluing) Warning: this is not tread safe
	if(inst->show_gnuplot > -1){
		if(inst->show_gnuplot > 0)
			sleep_ms(inst->show_gnuplot*1000);
					
		int** result1 = convertSolution(succ, comp, ncomp, inst);
		
		show_solution_comps(inst, true, result1, ncomp);
		sleep_ms(1000);
		
		for(int i=1; i<ncomp+1; i++) free(result1[i]);
		free(result1);
	}
	
	// glue components together
	mergeComponents(inst, &ncomp, comp, succ, objval);

	// convert to one-trip array
	int* trip = (int*) calloc(inst->nnodes, sizeof(int));
	int pos = 0;
	trip[0] = 0;

	for(int i=1;i<inst->nnodes;i++){
		trip[i] = succ[pos];
		pos = succ[pos];
	}

	// apply 2-opt
	inst->plotCosts = NULL;
	int nCosts = 0, xIndex = 0;
	twoOptLoop(inst, trip, objval, NULL, &nCosts, &xIndex, false, false, true);

	// DEBUG PLOTTING (after gluing & 2opt) Warning: this is not tread safe
	if(inst->show_gnuplot > -1){
		if(inst->show_gnuplot > 0)
			sleep_ms(inst->show_gnuplot*1000);

		int** result2 = convertSolution(succ, comp, ncomp, inst);
		
		show_solution_mono(inst, true, trip);
		sleep_ms(1000);

		for(int i=1; i<ncomp+1; i++) free(result2[i]);
		free(result2);
	}

	verbose_print(inst, 95, "[Posting Heuristic - Gluing] Ended merging and gluing2Opt, objval: %f ncomp %d\n", *objval, ncomp);

	// convert trip to cplex format
	int *ind = (int *) malloc(inst->ncols * sizeof(int));
	for ( int j = 0; j < inst->ncols; j++ ) ind[j] = j;

	double* xheu = (double*) calloc(inst->ncols, sizeof(double));

	for (int i = 0; i < inst->nnodes-1; i++) {
		int pos_index = xpos(trip[i], trip[i + 1], inst);
		xheu[pos_index] = 1;
	}

	xheu[xpos(trip[inst->nnodes-1], trip[0], inst)] = 1;

	// post solution
	int error = CPXcallbackpostheursoln(context, inst->ncols, ind, xheu, *objval, CPXCALLBACKSOLUTION_NOCHECK);
	assert(error == 0);
	
	free(xheu);
	free(ind);
	free(trip);

	return 0;
}

static int CPXPUBLIC candidateCallback(CPXCALLBACKCONTEXTptr context, instance* inst)
{ 
	// get instance and cplex solution
	int ncols = inst->ncols;
	double* xstar = (double*) malloc(ncols * sizeof(double));  
	double objval = CPX_INFBOUND; 
	
	int error = 0;
	error = CPXcallbackgetcandidatepoint(context, xstar, 0, ncols-1, &objval);
	assert(error == 0);
	
	// convert cplex solution into succ & comp
	int* succ = (int *) calloc(inst->nnodes, sizeof(int));
	int* comp = (int *) calloc(inst->nnodes, sizeof(int));
	int* dim = (int *) calloc(inst->nnodes + 1, sizeof(int));
	int ncomp;
	build_sol(xstar, inst, succ, comp, dim, &ncomp);

	verbose_print(inst, 95, "[CPLEX callback] Found solution with %d components with cost %f\n", ncomp, objval);
	
	// if cplex solution has multiple components, add SEC
	if(ncomp > 1){
		int izero = 0;
		char sense = 'L'; // <=
		int* index = (int*) calloc(ncols, sizeof(int)); // index of vars with coeff non-zero 
		double* value = (double*) calloc(ncols, sizeof(double)); // coeff values

		for(int k=1; k<=ncomp; k++){
			int nnz = 0; // number of non zero
			double rhs = -1.0; // right hand side of contraint
			
			// add constraint for component #k
			setConstraint(inst, index, value, comp, k, &rhs, &nnz);

			error = CPXcallbackrejectcandidate(context, 1, nnz, &rhs, &sense, &izero, index, value); // reject the solution and adds one cut 
			assert(error == 0); // CPXcallbackrejectcandidate error
		}

		free(value);
		free(index);

		// post heuristic with gluing
		if(inst->posting_base)
			posting_base(inst, context, succ, comp, ncomp, &objval);

	} else {
		// if cplex solution has 1 component, do nothing
		void* current_time = currentTime();
    	struct timespec end = *((struct timespec*)current_time);
		free(current_time);
	
    	double time = timeElapsed(&(inst->tstart), &(end));
		verbose_print(inst, 80, "[CPLEX callback] Found feasible solution with cost %f after %f seconds\n", objval, time);

		// plot
		if(inst->show_gnuplot > -1){
			if(inst->show_gnuplot > 0)
				sleep_ms(inst->show_gnuplot*1000);

			int** result = convertSolution(succ, comp, ncomp, inst);
			show_solution_comps(inst, true, result, ncomp);

			for(int i=1; i<ncomp+1; i++) free(result[i]);
			free(result);
		}
	}	

	free(dim);
	free(comp);
	free(succ);
	free(xstar);
	return 0; 
}

int add_cut(double cut_value, int cut_nnodes, int* cut_index_nodes, void* userhandle)
{	
	cut_par cut_pars = *(cut_par*) userhandle;
	instance* inst = cut_pars.inst;

	int* index = calloc(cut_nnodes*(cut_nnodes-1)/2,sizeof(int));
	double* value = calloc(cut_nnodes*(cut_nnodes-1)/2,sizeof(double));

	int izero = 0;
	int purgeable = CPX_USECUT_FILTER;
	int local = 0;
	double rhs = -1.0;
	char sense = 'L';
	int nnz = 0;

	for(int i = 0; i<cut_nnodes;i++){
		rhs++;
		for(int j =i+1;j < cut_nnodes; j++){
			index[nnz]=xpos(cut_index_nodes[i], cut_index_nodes[j], inst);
			value[nnz]=1.0;
			nnz++;
		}
	}
	
	int error = CPXcallbackaddusercuts(cut_pars.context, 1, nnz, &rhs, &sense, &izero, index, value, &purgeable, &local);
	assert(error == 0);// CPXcallbackaddusercuts error

	free(index);
	free(value);
	return 0; 
}

int posting_relax(CPXCALLBACKCONTEXTptr context, instance* inst, double* xstar, double* objval){
	for(int i=0;i<inst->ncols; i++){
		xstar[i] = 1 - (0.25 + (xstar[i] * 0.5));
	}

	// Nearest Neighbor using xstar as weights
	int* result = (int*) calloc(inst->nnodes, sizeof(int));
	WeightedNNFromEachNode(inst, xstar, result, objval, false);

	// apply 2-opt
	inst->plotCosts = NULL;
	int nCosts = 0, xIndex = 0;
	twoOptLoop(inst, result, objval, NULL, &nCosts, &xIndex, false, false, true);

	// convert trip to cplex format
	int *ind = (int *) malloc(inst->ncols * sizeof(int));
	for ( int j = 0; j < inst->ncols; j++ ) ind[j] = j;

	double* xheu = (double*) calloc(inst->ncols, sizeof(double));

	for (int i = 0; i < inst->nnodes-1; i++) {
		int pos_index = xpos(result[i], result[i + 1], inst);
		xheu[pos_index] = 1;
	}

	xheu[xpos(result[inst->nnodes-1], result[0], inst)] = 1;

	// post solution
	int error = CPXcallbackpostheursoln(context, inst->ncols, ind, xheu, *objval, CPXCALLBACKSOLUTION_NOCHECK);
	assert(error == 0);
	
	free(xheu);
	free(ind);
	free(result);
	return 0;
}

static int CPXPUBLIC relaxationCallback(CPXCALLBACKCONTEXTptr context, instance* inst)
{ 
	if(randomInt(1, 10) != 1) return 0;

	// get instance and cplex fractional solution
	int ncols = inst->ncols;
	double* xstar = (double*) malloc(ncols * sizeof(double));  
	double objval = CPX_INFBOUND; 
	
	int error = 0;
	error = CPXcallbackgetrelaxationpoint(context, xstar, 0, ncols-1, &objval);
	assert(error == 0);

	// count number of components
	int* elist = (int *) calloc(2*ncols, sizeof(int));
	int ncomp = -1;
	int* compscount = (int*) NULL;
	int* comps = (int*) NULL;

    int k=0;
	for(int i = 0; i<inst->nnodes; i++){
		for(int j=i+1;j<inst->nnodes;j++){
			elist[k]=i;
			elist[++k]=j;
			k++;
		}
	}

	CCcut_connect_components(inst->nnodes, ncols, elist, xstar, &ncomp, &compscount, &comps);

	if(ncomp > 1){ // if there is more than 1 component add SEC

		int izero = 0;
		char sense = 'L'; // <=
		int* index = (int*) calloc(ncols, sizeof(int)); // index of vars with coeff non-zero 
		double* value = (double*) calloc(ncols, sizeof(double)); // coeff values
		int purgeable = CPX_USECUT_FILTER;
		int local = 0;

		for(int k=1; k<=ncomp; k++){
			int nnz = 0; // number of non zero
			double rhs = -1.0; // right hand side of contraint
			
			// add constraint for component #k
			setConstraint(inst, index, value, comps, k, &rhs, &nnz);

			error = CPXcallbackaddusercuts(context, 1, nnz, &rhs, &sense, &izero, index, value, &purgeable, &local); // adds one cut
			assert(error == 0); // CPXcallbackrejectcandidate error
		}

		free(value);
		free(index);

	} else {
		int* elist = (int*) malloc(2*ncols*sizeof(int));  
		int ncomp = -1;

		int k=0;
		for(int i = 0;i<inst->nnodes;i++){
			for(int j=i+1;j<inst->nnodes;j++){
				elist[k]=i;
				elist[++k]=j;
				k++;
			}
		}
		
		cut_par user_handle = {context,inst};
		error = CCcut_violated_cuts(inst->nnodes, ncols, elist, xstar, 1.9, add_cut, (void*) &user_handle);
		
		free(elist);
	}

	if(inst->posting_relax){
		posting_relax(context, inst, xstar, &objval);
	}

	free(elist);
	free(compscount);
	free(comps);
	free(xstar);
	return 0; 
}
