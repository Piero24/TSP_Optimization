#include "Algorithm/cplexAlg.h"

double dist(int i, int j, instance *inst)
{
	return inst->distances[i][j];
}

int TSPopt(instance *inst)
{  

	// open CPLEX model
	int error;
	CPXENVptr env = CPXopenCPLEX(&error);
	CPXLPptr lp = CPXcreateprob(env, &error, "TSP"); 

	build_model(inst, env, lp);
	
	// Cplex's parameter setting
	if ( CPXmipopt(env,lp) ) print_error("CPXmipopt() error");    

	// get the optimal solution found by CPLEX
	int ncols = CPXgetnumcols(env, lp); //n*(n-1)/2
	double *xstar = (double *) calloc(ncols, sizeof(double));
    double objval = 0;

    //CPXgetx(env, lp, xstar, 0, ncols-1)
	if ( CPXsolution(env, lp, NULL, &objval, xstar, NULL, NULL, NULL) ) print_error("CPXgetx() error");	
    
    // print solution
    if(inst->verbose >= 100){
        printf("costs:%f\n", objval);
        for ( int i = 0; i < inst->nnodes; i++ )
        {
            for ( int j = i+1; j < inst->nnodes; j++ )
            {
                if ( xstar[xpos(i,j,inst)] > 0.5 ) printf("x(%3d,%3d) = 1\n", i+1,j+1); // print dei lati scelti
                    // xstar non è mai precisamente 0 o 1
            }
        }
    }

    int* result = (int *) calloc(inst->nnodes, sizeof(int));
    convertSolution(xstar, result, inst);
    bestSolution(result, objval, inst);
	
	// free and close cplex model   
    free(xstar);
    free(result);
	CPXfreeprob(env, &lp);
	CPXcloseCPLEX(&env); 

	return 0; // or an appropriate nonzero error code

}

void convertSolution(double *xstar, int* result, instance* inst){
    int currentPos = 0, lastNode = 0, currentNode = 0;
    
    result[currentPos++] = currentNode;
    
    while(currentPos < inst->nnodes){
        for(int i=0;i<inst->nnodes;i++){
            if(i != lastNode && i != currentNode && xstar[xpos(currentNode, i, inst)] > 0.5){
                lastNode = currentNode;
                currentNode = i;
                result[currentPos++] = i;
                break;
            }
        }
    }
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
	
	char **cname = (char **) calloc(1, sizeof(char *));		// (char **) required by cplex...
	cname[0] = (char *) calloc(100, sizeof(char));

	// add binary var.s x(i,j) for i < j  
	for ( int i = 0; i < inst->nnodes; i++ )
	{
		for ( int j = i+1; j < inst->nnodes; j++ )
		{
			sprintf(cname[0], "x(%d,%d)", i+1,j+1);  // x(1,2), x(1,3) ....
			double obj = dist(i,j,inst); // cost == distance   
			double lb = 0.0;
			double ub = 1.0;

			if ( CPXnewcols(env, lp, 1, &obj, &lb, &ub, &binary, cname) ) print_error(" wrong CPXnewcols on x var.s");
    		if ( CPXgetnumcols(env,lp)-1 != xpos(i,j, inst) ) print_error(" wrong position for x var.s");
		}
	} 

	// add degree constr.s 

	int *index = (int *) malloc(inst->nnodes * sizeof(int));
	double *value = (double *) malloc(inst->nnodes * sizeof(double));  // coefficienti per indicare quali lati sto controllando (tutti quelli collegati con h tranne h stesso)
	
	// add the degree constraints
	for ( int h = 0; h < inst->nnodes; h++ )  // degree constraints
	{
		double rhs = 2.0;
		char sense = 'E';                     // 'E' for equality constraint 
		sprintf(cname[0], "degree(%d)", h+1); 
		int nnz = 0;
		for ( int i = 0; i < inst->nnodes; i++ )
		{
			if ( i == h ) continue;
			index[nnz] = xpos(i,h, inst);
			value[nnz] = 1.0;
			nnz++;
		}
		
		if ( CPXaddrows(env, lp, 0, 1, nnz, &rhs, &sense, &izero, index, value, NULL, &cname[0]) ) print_error(" wrong CPXaddrows [degree]");
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

/*
#define DEBUG    // da commentare se non si vuole il debugging
#define EPS 1e-5

/*********************************************************************************************************************************/
//void build_sol(const double *xstar, instance *inst, int *succ, int *comp, int *ncomp) // build succ() and comp() wrt xstar()...
/*********************************************************************************************************************************/
/*
{   

#ifdef DEBUG
	int *degree = (int *) calloc(inst->nnodes, sizeof(int));
	for ( int i = 0; i < inst->nnodes; i++ )
	{
		for ( int j = i+1; j < inst->nnodes; j++ )
		{
			int k = xpos(i,j,inst);
			if ( fabs(xstar[k]) > EPS && fabs(xstar[k]-1.0)) > EPS ) print_error(" wrong xstar in build_sol()");
			if ( xstar[k] > 0.5 ) 
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