#ifndef TSP_H
#define TSP_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>

#ifdef _WIN32
    #include <Windows.h>
#endif

#define EPS 1e-5

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
	bool debug;
	int ncols;								// cplex number of columns

	//global data
	struct timespec tstart;					// starting time
	struct timespec tbest;					// time for the best sol. available
	double zbest;							// cost best sol. available  
	int *best_sol;							// best sol. available (WARNING: was double)
	double best_lb;							// best lower bound available  
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
    bool plotFlag;
	FILE *plotSolution;
	FILE *plotCosts;

	char name_csv[250];
	
	char file_name[50];
	char file_comment[50];

	bool mipstart;
	bool callback_base;
	bool callback_relax; 
	bool posting_base;
	bool posting_relax;

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
 * @brief Returns the filename from a given file path.
 * 
 * @param filePath The file path from which to extract the filename.
 * @param extension The extension to be removed from the filename.
 * 
 * @return The filename extracted from the file path.
 */
char *getFileName(const char *filePath, const char *extension);

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
 * @brief Plots the costs over time using Gnuplot.
 * 
 * @param plot A pointer to the gnuplot file.
 * @param costs A vector of costs over time.
 * @param n The number of costs in the vector.
 * @param alg True if the function is called by an algorithm, false if it is
 * 	called by an optimization method.
 */
void show_costs(instance* inst, point* costs, int n, bool alg);

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
 * @brief Generate a random int between min and max
 * 
 * @param min The minimum value of the random number.
 * @param max The maximum value of the random number.
 * 
 * @return A random int between min and max.
 */
int randomInt(int min, int max);

/**
 * @brief Generate a random bool
 * 
 * @return A random bool
 */
bool randomBool();

/**
 * @brief Generate a .tsp file with n random nodes.
 * The file it will be saved in the Resource folder.
 * 
 * @param n The number of nodes to generate.
 * @param seed The random seed of the file to generate.
 * 
 * @return The name of the file generated.
 */
char* fileGenerator(int n, int seed);

/**
 * @brief Return distance between node a and b.
 * 
 * @param inst A pointer to the instance structure to be freed.
 * @param a The index of the first node.
 * @param b The index of the second nod.
 * 
 * @return The distance between node a and b.
 */
double dist(instance* inst, int a, int b);

/**
 * @brief Prints the solution to the standard output or generates a plot using Gnuplot, 
 * depending on the specified flag. Works with multiple components solutions.
 * 
 * @param inst A pointer to the instance structure containing the solution data.
 * @param useGnuplot A boolean flag indicating whether to generate a plot 
 * using Gnuplot (true) or print the solution to the standard output (false).
 * @param result A pointer to the array of paths (aka array of array of nodes).
 * @param ncomp The number of components.
 */
void show_solution_comps(instance* inst, bool useGnuplot, int** result, int ncomp);

/**
 * @brief Prints the solution to the standard output or generates a plot using Gnuplot, 
 * depending on the specified flag. Works with one single component that is not in the instance struct.
 * 
 * @param inst A pointer to the instance structure containing the solution data.
 * @param useGnuplot A boolean flag indicating whether to generate a plot 
 * using Gnuplot (true) or print the solution to the standard output (false).
 * @param result A pointer to the array of nodes.
 */
void show_solution_mono(instance* inst, bool useGnuplot, int* result);

/**
 * @brief Print the formatted string if inst->verbose >= vMin.
 * 
 * @param inst A pointer to the instance structure.
 * @param vMin Minimum verbosity required for printing.
 * @param format The format string.
 * @param ... The list of arguments of the format string.
 */
void verbose_print(instance *inst, int vMin, const char* format, ...);

#endif /* TSP_H */