#include <float.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vrp.h"
//#include <cplex.h> //CPX_INFBOUND

/**
 * @brief Parses command-line arguments to initialize the instance structure.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @param inst A pointer to the instance structure to be initialized.
 * 
 * @return 0 on success, non-zero on failure.
 */
int parse_args(int argc, char** argv, instance* inst);

/**
 * @brief Reads and parses the input data from the specified input file, 
 * initializing the instance structure accordingly.
 * 
 * @param inst A pointer to the instance structure where the parsed data will be stored.
 * 
 * @return 0 on success, non-zero on failure.
 */
int read_input(instance* inst);