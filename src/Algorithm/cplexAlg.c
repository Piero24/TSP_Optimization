#include "Algorithm/cplexAlg.h"

#define DEBUG    // da commentare se non si vuole il debugging
#define EPS 1e-5

int TSPopt(instance *inst)
{  
	if(inst->verbose >= 60) printf("[CPLEX] Initializing algorithm...\n");

	// open CPLEX model
	int error;
	CPXENVptr env = CPXopenCPLEX(&error);
	CPXLPptr lp = CPXcreateprob(env, &error, "TSP"); 

	build_model(inst, env, lp);
	
	// Cplex's get best sol
	if(inst->verbose >= 60) printf("[CPLEX] Getting best solution\n");
	if ( CPXmipopt(env,lp) ) print_error("CPXmipopt() error");    

	// get info of the optimal solution found by CPLEX
	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
	double *xstar = (double *) calloc(ncols, sizeof(double));
    double objval = 0;
	
    CPXgetbestobjval(env, lp, &objval);
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
					// xstar itìs never 0 o 1 precisely
					printf("[CPLEX] x(%3d,%3d) = 1\n", i+1,j+1);
				}
            }
        }
    }

    int** result;
	int ncomp;
	int* succ = (int *) calloc(inst->nnodes, sizeof(int));
	int* comp = (int *) calloc(inst->nnodes, sizeof(int));

	inst->tbest = clock();
	inst->best_lb = objval;
	
	build_sol(xstar, inst, succ, comp, &ncomp);
	result = convertSolution(succ, comp, ncomp, inst);
	show_solution_comps(inst, true, result, ncomp);
	
	// free and close cplex model   
    for(int i=1; i<ncomp+1; i++) free(result[i]);
	free(result);
    free(xstar);
    free(succ);
    free(comp);
	CPXfreeprob(env, &lp);
	CPXcloseCPLEX(&env); 

	return 0; // or an appropriate nonzero error code

}

int** convertSolution(int *succ, int *comp, int ncomp, instance* inst)
{
	if(inst->verbose >= 95) printf("[convertSolution] Creating result array.\n");

	int** result = (int **) calloc(ncomp+1, sizeof(int*));
	for(int i=1; i<ncomp+1; i++){
		result[i] = (int *) calloc(inst->nnodes, sizeof(int));

		for(int j=0; j<inst->nnodes; j++){
			result[i][j] = -1;
		}
	}

	if(inst->verbose >= 95) printf("[convertSolution] Creating indexes array.\n");

	int* indexes = (int *) calloc(ncomp+1, sizeof(int));
	for(int i=0; i<ncomp+1; i++) indexes[i] = 0;

	if(inst->verbose >= 95) printf("[convertSolution] Starting converting. ncomp %d\n", ncomp);

    for(int i=0; i<inst->nnodes; i++){
		if(indexes[comp[i]] == 0){
			result[comp[i]][0] = i;
			indexes[comp[i]]++;
			int next = succ[i];

			while(next != i){
				result[comp[i]][indexes[comp[i]]] = next;
				indexes[comp[i]]++;
				next = succ[next];
			}
		}
	}

	if(inst->verbose >= 95)
		for(int i=1; i<ncomp; i++){
			printf("[convertSolution]");
			for(int j=0; j<inst->nnodes && result[i][j] != -1; j++){
				printf(" result[%d][%d] %d\t", i, j, result[i][j]);
			}
			printf("\n");
		}


	free(indexes);
	return result;
}

int xpos(int i, int j, instance *inst)
{ 
	if ( i == j ) print_error(" i == j in xpos" );
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

			if ( CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname) ) print_error(" wrong CPXnewcols on x var.s");
    		if ( CPXgetnumcols(env,lp)-1 != xpos(i,j, inst) ) print_error(" wrong position for x var.s");
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
		
		if ( CPXaddrows(env, lp, 0, 1, nnz, &rhs, &sense, &izero, index, value, NULL, &cname[0]) )
		{
			print_error(" wrong CPXaddrows [degree]");
		}
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

void build_sol(const double *xstar, instance *inst, int *succ, int *comp, int *ncomp) // build succ() and comp() wrt xstar()...
{   

#ifdef DEBUG
	int *degree = (int *) calloc(inst->nnodes, sizeof(int));
	for ( int i = 0; i < inst->nnodes-1; i++ )
	{
		for ( int j = i+1; j < inst->nnodes; j++ )
		{
			int k = xpos(i,j,inst);
			if (fabs(xstar[k]) > EPS && fabs(xstar[k]-1.0) > EPS){
				printf("i %d j %d xpos %d\n", i, j, k);
				printf("fabs(xstar[k]) %f, fabs(xstar[k]-1.0) %f\n", fabs(xstar[k]), fabs(xstar[k]-1.0));
				print_error(" wrong xstar in build_sol()");
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
		if ( degree[i] != 2 ) print_error("wrong degree in build_sol()");
	}	
	free(degree);
#endif

	*ncomp = 0;
	for ( int i = 0; i < inst->nnodes; i++ )
	{
		succ[i] = -1;
		comp[i] = -1;
	}
	
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

void add_SEC(instance* inst, CPXENVptr env, CPXLPptr lp, int ncomp, int* comp){
	if(ncomp == 1) print_error("add_SEC called with ncomp=1");
	
	int izero = 0;
	int ncols = CPXgetnumcols(env, lp);
	int* index = (int*) calloc(ncols, sizeof(int)); // indici delle variabili con coefficiente diverso da zero 
	double* value = (double*) calloc(ncols, sizeof(double)); // valore dei coefficienti della sommatoria
	char **cname = (char **) calloc(1, sizeof(char *));
	cname[0] = (char *) calloc(100, sizeof(char));

	for(int k=1; k<=ncomp; k++){
		int nnz = 0; // number of non zero
		char sense = 'L'; // <=
		double rhs = -1.0; // right hand side of contraint
		sprintf(cname[0], "component(%d)", k+1); 
		
		// aggiungo vincolo per component #k
		for(int i=0; i<inst->nnodes; i++){
			if(comp[i] != k) continue;
			rhs += 1;
			
			for(int j=i+1; j<inst->nnodes; j++){
				if(comp[j] != k) continue;
					
				index[nnz] = xpos(i,j, inst);
				value[nnz] = 1.0;
				nnz ++;
			}
		}
		
		CPXaddrows(env, lp, 0, 1, nnz, &rhs, &sense, &izero, index, value, NULL, cname);
	}

	free(cname[0]);
	free(cname);
	free(index);
	free(value);
}

void mergeComponents(instance* inst, int* ncomp, int* comp, int *succ, double *cost){
	if(inst->verbose >= 90) printf("[mergeComponents] Starting the merge of %d components\n", *ncomp);
	int count = 0;
	while(*ncomp > 1){
		if(inst->verbose >= 95) printf("[mergeComponents] Current #components: %d\n", *ncomp);

		// find best swap
		int bestA = -1, bestB = -1;
		double bestDiffC = DBL_MAX;
		for(int A=0;A<inst->nnodes-1;A++){
			for(int B=A+1;B<inst->nnodes;B++){
				if(comp[A] == comp[B])
					continue;
				
				int A1 = succ[A], B1 = succ[B];
				double diffC = - dist(inst, A, A1) - dist(inst, B, B1) + dist(inst, A, B1) + dist(inst, B, A1);
				
				if(diffC < bestDiffC){
					bestDiffC = diffC;
					bestA = A;
					bestB = B;
				}
			}
		}


		// do best swap
		int A1 = succ[bestA], B1 = succ[bestB];
		
		if(inst->verbose >= 95) printf("[mergeComponents] Best merging found: A: %d\tA1: %d\tB: %d\tB1: %d\n", bestA, A1, bestB, B1);
		if(inst->verbose >= 95) printf("[mergeComponents] Best merging from A->A1 and B->B1 to A->B1 and B->A1\n");
		if(inst->verbose >= 95) printf("[mergeComponents] Best merging has diffC: %f\n", bestDiffC);

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
		
		if(bestDiffC < 0 && count == 0) {printf("[mergeComponents] Error: diffC < 0 diffC: %f\n", bestDiffC);exit(0);}

		*cost += bestDiffC;
		
		*ncomp = *ncomp - 1;
		count ++;

		if(inst->verbose >= 95) printf("[mergeComponents] Best merging done. ncomp %d\n", *ncomp);
		
		//int** result = convertSolution(succ, comp, *ncomp, inst);
		//if(inst->verbose >= 95) printf("[mergeComponents] convertSolution done.\n");
		//show_solution_comps(inst, true, result, *ncomp);
		//if(inst->verbose >= 95) printf("[mergeComponents] show_solution_comps done.\n");

		//for(int i=1; i<*ncomp+1; i++) free(result[i]);
		//free(result);
	}
}

int bendersLoop(instance *inst, bool gluing)
{  
	if(inst->verbose >= 60) printf("[Benders] Initializing algorithm...\n");

	// open and build CPLEX model
	int error, ncomp;
	CPXENVptr env = CPXopenCPLEX(&error);
	CPXLPptr lp = CPXcreateprob(env, &error, "TSP"); 
	build_model(inst, env, lp);

	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
    double objval = 0;
	int* succ = (int *) calloc(inst->nnodes, sizeof(int));
	int* comp = (int *) calloc(inst->nnodes, sizeof(int));
	double *xstar = (double *) calloc(ncols, sizeof(double));

	if(inst->verbose >= 60) printf("[Benders] Algorithm initialized, starting processing...\n");
	
	//first check of time
	clock_t end = clock();
    double time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;
	
	do{
		//set time limit
		CPXsetdblparam(env, CPXPARAM_TimeLimit, inst->time_limit - time);

		//compute solution
		if ( CPXmipopt(env,lp) ) print_error("CPXmipopt() error");
		CPXgetbestobjval(env, lp, &objval);
		CPXgetx(env, lp, xstar, 0, ncols - 1);

		//save lower bound
		if(inst->best_lb > objval)
			inst->best_lb = objval;
		
		// compute connected components
		build_sol(xstar, inst, succ, comp, &ncomp);

		if(inst->verbose >= 80) printf("[Benders] Found solution with %d components with cost %f\n", ncomp, objval);
		
		// if there is only 1 connected compotent the solution is found
		if(ncomp == 1) break;

		// otherwise add sub-tour elimination contraint
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

				if(inst->show_gnuplot > 0)
					sleep_ms(inst->show_gnuplot*1000);
			}
			
			gluing2Opt(inst, result[1], objval);

			if(inst->show_gnuplot > -1)
				show_solution_comps(inst, true, result, ncomp);

			if(inst->verbose >= 80) printf("[Benders - Gluing] Merged solution has cost %f\n", objval);

			if(objval < inst->zbest)
				bestSolution(result[1], objval, inst);
			
			for(int i=1; i<ncomp+1; i++) free(result[i]);
			free(result);
		} else if(inst->show_gnuplot > -1){
			int** result = convertSolution(succ, comp, ncomp, inst);
			show_solution_comps(inst, true, result, ncomp);

			if(inst->show_gnuplot > 0)
				sleep_ms(inst->show_gnuplot*1000);
		}

		//check time
		end = clock();
        time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;
	}while(time < inst->time_limit);
	
    // print solution
    if(inst->verbose >= 100){
        printf("costs:%f\n", objval);

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

	if(ncomp == 1){
		int** result = convertSolution(succ, comp, ncomp, inst);

		bestSolution(result[1], objval, inst);
		show_solution_comps(inst, true, result, ncomp);

		for(int i=1; i<ncomp+1; i++) free(result[i]);
		free(result);
	}else if(inst->verbose >= 60){
		printf("[Benders] solution not found");
	}
	
	// free and close cplex model   
    free(xstar);
	free(succ);
	free(comp);
	CPXfreeprob(env, &lp);
	CPXcloseCPLEX(&env); 

	return 0; // or an appropriate nonzero error code

}

int gluing2Opt(instance* inst, int* result, double cost)
{   
    inst->plotCosts = NULL;
    int nCosts = 0, xIndex = 0;

    if (inst->verbose >= 90) printf("[Gluing - 2opt] Starting optimization.\n");

    twoOptLoop(inst, result, &cost, NULL, &nCosts, &xIndex, false, false);

    if (inst->verbose >= 90) printf("[Gluing - 2opt] Optimization completed.\n\n");

    bestSolution(result, cost, inst);
    
    return 0;
}

/*
**** LAZY CONTRAINTS ****

Ex: MZT formulation with directed-arc variables x_ij and x_ji --> xpos_compact(i,j,inst)

...

	int izero = 0;
	int index[3]; 
	double value[3];

	// add lazy constraints  1.0 * u_i - 1.0 * u_j + M * x_ij <= M - 1, for each arc (i,j) not touching node 0	
	double big_M = inst->nnodes - 1.0;
	double rhs = big_M -1.0;
	char sense = 'L';
	int nnz = 3;
	for ( int i = 1; i < inst->nnodes; i++ ) // excluding node 0
	{
		for ( int j = 1; j < inst->nnodes; j++ ) // excluding node 0 
		{
			if ( i == j ) continue;
			sprintf(cname[0], "u-consistency for arc (%d,%d)", i+1, j+1);
			index[0] = upos(i,inst);	
			value[0] = 1.0;	
			index[1] = upos(j,inst);
			value[1] = -1.0;
			index[2] = xpos_compact(i,j,inst);
			value[2] = big_M;
			if ( CPXaddlazyconstraints(env, lp, 1, nnz, &rhs, &sense, &izero, index, value, cname) ) print_error("wrong CPXlazyconstraints() for u-consistency");
		}
	}
	
	// add lazy constraints 1.0 * x_ij + 1.0 * x_ji <= 1, for each arc (i,j) with i < j
	rhs = 1.0; 
	char sense = 'L';
	nnz = 2;
	for ( int i = 0; i < inst->nnodes; i++ ) 
	{
		for ( int j = i+1; j < inst->nnodes; j++ ) 
		{
			sprintf(cname[0], "SEC on node pair (%d,%d)", i+1, j+1);
			index[0] = xpos_compact(i,j,inst);
			value[0] = 1.0;
			index[1] = xpos_compact(j,i,inst);
			value[1] = 1.0;
			if ( CPXaddlazyconstraints(env, lp, 1, nnz, &rhs, &sense, &izero, index, value, cname) ) print_error("wrong CPXlazyconstraints on 2-node SECs");
		}
	}

...

*** SOME CPLEX'S PARAMETERS ***


	// increased precision for big-M models
	CPXsetdblparam(env, CPX_PARAM_EPINT, 0.0);		// very important if big-M is present
	CPXsetdblparam(env, CPX_PARAM_EPRHS, 1e-9);   						

	CPXsetintparam(env, CPX_PARAM_MIPDISPLAY, 4);
	if ( VERBOSE >= 60 ) CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_ON); // Cplex output on screen
	CPXsetintparam(env, CPX_PARAM_RANDOMSEED, 123456);
	
	CPXsetdblparam(env, CPX_PARAM_TILIM, CPX_INFBOUND+0.0); 
	
	CPXsetintparam(env, CPX_PARAM_NODELIM, 0); 		// abort Cplex after the root node
	CPXsetintparam(env, CPX_PARAM_INTSOLLIM, 1);	// abort Cplex after the first incumbent update
	CPXsetdblparam(env, CPX_PARAM_EPGAP, 1e-4);  	// abort Cplex when gap below 10%	 
*/