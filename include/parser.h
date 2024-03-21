#include <float.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "tsp.h"
#include "menu.h"
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

/**
 * @brief Return the time limit in string format.
 * 
 * @param time_limit The time limit in seconds.
 * 
 * @return The time limit in string format (inf if is equal to DBL_MAX or CPX_INFBOUND).
 */
const char* getTimeLimitString(double time_limit);

/**
 * @brief 
 * 
 * @param inst
 */
void parameterPrint(instance* inst);