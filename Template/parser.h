#include <float.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vrp.h"
//#include <cplex.h> //CPX_INFBOUND

#define VERBOSE 5000

int parse_args(int argc, char** argv, instance* inst);
int read_input(instance* inst);
void print_error(const char *err);