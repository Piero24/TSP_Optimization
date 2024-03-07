#ifndef VRP_H
#define VRP_H

#include <stdbool.h>

#define VERBOSE 500

typedef struct {
	double x, y;
} point;

typedef struct {   
	
	//input data
	int nnodes; 	
	//double *demand;   
	point *coord;
	double **distances;
	int depot;
	double capacity; 
	int nveh;

	// parameters 
	int model_type; 
	int old_benders;
	int randomseed;
	int num_threads;
	double timelimit;						// overall time limit, in sec.s
	char input_file[1000];		  			// input file
	char node_file[1000];		  			// cplex node file
	int available_memory;
	int max_nodes; 							// max n. of branching nodes in the final run (-1 unlimited)
	double cutoff; 							// cutoff (upper bound) for master
	int integer_costs;

	//global data
	// double	tstart;								
	double zbest;							// best sol. available  
	double tbest;							// time for the best sol. available  
	int *best_sol;							// best sol. available (WARNING: was double)
	// double	best_lb;					// best lower bound available  
	// double *load_min;					// minimum load when leaving a node
	// double *load_max;					// maximum load when leaving a node
	
	// model;     
	// int xstart;
	// int qstart;
	// int bigqstart;  						
	// int sstart;
	// int bigsstart;
	// int ystart;
	// int fstart;
	// int zstart;
} instance;

/**
 * @brief Prints the solution to the standard output or generates a plot using Gnuplot, 
 * depending on the specified flag.
 * 
 * @param inst A pointer to the instance structure containing the solution data.
 * @param useGnuplot A boolean flag indicating whether to generate a plot 
 * using Gnuplot (true) or print the solution to the standard output (false).
 */
void print_solution(instance* inst, bool useGnuplot);

/**
 * @brief Frees the memory allocated for the instance structure.
 * 
 * @param inst A pointer to the instance structure to be freed.
 */
void free_instance(instance* inst);

#endif /* VRP_H */