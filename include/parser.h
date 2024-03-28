#ifndef PARSER_H
#define PARSER_H

#include <float.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "tsp.h"
#include "menu.h"

#include <cplex.h> //CPX_INFBOUND

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
 * @brief Prints the selected parameters of an instance.
 * 
 * @param inst Pointer to the instance structure containing the parameters to print.
 */
void parameterPrint(instance* inst);

/**
 * @brief Assigns a model name to the instance based on the provided type.
 * 
 * This function assigns a model name to the instance based on the specified type.
 * If the provided type is:
 * - 1: "Nearest Neighbor" is assigned as the algorithm name.
 * - 2: "Variable Neighborhood Search" is assigned as the algorithm name.
 * - 3: "Algo 3" is assigned as the algorithm name.
 * If the type is none of the above, a random type between 1 and 3 is generated 
 * and the function recursively calls itself with the generated type.
 * 
 * @param inst Pointer to the instance structure.
 * @param type The type of the model.
 * @return Returns 0 upon successful assignment.
 */
int intToModelName(instance* inst, int type);

/**
 * @brief Assigns an optimization name to the instance based on the provided type.
 * 
 * This function assigns an optimization name to the instance based on the specified type.
 * If the provided type is:
 * - 1: "2-Opt" is assigned as the optimization name.
 * - 2: "Tabu Search" is assigned as the optimization name.
 * - 3: "Opt 3" is assigned as the optimization name.
 * If the type is none of the above, "None" is assigned as the optimization name.
 * 
 * @param inst Pointer to the instance structure.
 * @param type The type of the optimization.
 * @return Returns 0 upon successful assignment.
 */
int intToOptName(instance* inst, int type);

/**
 * @brief Prints an error message to the standard output and exits the program.
 * 
 * @param err The error message to be printed.
 */
void print_error(const char *err);

/**
 * @brief Returns the filename from a given file path.
 * 
 * @param filePath The file path from which to extract the filename.
 * 
 * @return The filename extracted from the file path.
 */
char *getFileName(const char *filePath);

/**
 * @brief Return the time limit in string format.
 * 
 * @param time_limit The time limit in seconds.
 * 
 * @return The time limit in string format (inf if is equal to DBL_MAX or CPX_INFBOUND).
 */
const char* getTimeLimitString(double time_limit);

/**
 * @brief Reads only part of the input file for extract essential parameters.
 * 
 * @param inst A pointer to the instance structure where the parsed data will be stored.
 * 
 * @return 0 on success, non-zero on failure.
 */
int readInputParameters(instance* inst);

/**
 * @brief Reads and parses the input data from the specified input file, 
 * initializing the instance structure accordingly.
 * 
 * @param inst A pointer to the instance structure where the parsed data will be stored.
 * 
 * @return 0 on success, non-zero on failure.
 */
int read_input(instance* inst);

#endif /* PARSER_H */