#ifndef TSP_H
#define TSP_H

#include <stdbool.h>
#include <string.h> 

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
	int old_benders;
	int random_seed;
	int num_threads;
	double time_limit;						// overall time limit, in sec.s
	char input_file[1000];		  			// input file
	char node_file[1000];		  			// cplex node file
	int available_memory;
	int max_nodes; 							// max n. of branching nodes in the final run (-1 unlimited)
	double cutoff; 							// cutoff (upper bound) for master
	int integer_costs;
	int verbose;							// range 0-100

	//global data
	double tstart;							// starting time		
	double zbest;							// cost best sol. available  
	double tbest;							// time for the best sol. available  
	int *best_sol;							// best sol. available (WARNING: was double)
	// double	best_lb;					// best lower bound available  
	// double *load_min;					// minimum load when leaving a node
	// double *load_max;					// maximum load when leaving a node
	
	// model;     
	int start;
	// int qstart;
	// int bigqstart;  						
	// int sstart;
	// int bigsstart;
	// int fstart;
	// int zstart;

	char algorithm_name[50];
    char opt_name[50];
    int show_gnuplot;
	char file_name[50];
	char file_comment[50];

} instance;

/**
 * @brief Generates a data file with node coordinates for 
 * the specified instance and saves it to the given filename.
 * 
 * @param filename The name of the file to write the data to.
 * @param inst A pointer to the instance structure containing node coordinates.
 */
void generateDataFile(const char* filename, instance* inst);

/**
 * @brief Prints the solution to the standard output or generates a plot using Gnuplot, 
 * depending on the specified flag.
 * 
 * @param inst A pointer to the instance structure containing the solution data.
 * @param useGnuplot A boolean flag indicating whether to generate a plot 
 * using Gnuplot (true) or print the solution to the standard output (false).
 */
void show_solution(instance* inst, bool useGnuplot);

/**
 * @brief Save the solution in a file ....
 * 
 * @param inst A pointer to the instance structure containing the solution data.
 * @param outputFileName The name of the file to write the solution to.
 */
void save_solution(instance* inst);

/**
 * @brief Frees the memory allocated for the instance structure.
 * 
 * @param inst A pointer to the instance structure to be freed.
 */
void free_instance(instance* inst);

/**
 * @brief Generate a random double between min and max
 * 
 * @param min The minimum value of the random number.
 * @param max The maximum value of the random number.
 * 
 * @return A random double between min and max.
 */
double randomDouble(double min, double max);

/**
 * @brief Generate a .tsp file with n random nodes.
 * The file it will be saved in the Resource folder.
 * 
 * @param n The number of nodes to generate.
 * 
 * @return The name of the file generated.
 */
char* fileGenerator(int n);

#endif /* TSP_H */