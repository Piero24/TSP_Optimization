#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>


#ifdef _WIN32
    #include <Windows.h>
#endif

#include "tsp.h"
#include "parser.h"
#include "algoSelector.h"


/**
 * @brief 
 * 
 * @param inst
 * @param filename
 * 
 * @return 
 */
int manage_launcher(instance *inst, const char *filename);

/**
 * @brief 
 * 
 * @param inst
 * 
 * @return
 */
int execute_workflow(instance *inst);

#endif /* LAUNCHER_H */