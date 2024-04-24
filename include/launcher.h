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


#define MAX_FILES 10
#define MAX_HEADER_LENGTH 350

typedef struct {
	char algorithm[50];
    char optimizer[50];
} Tuple;

/**
 * @brief 
 * 
 * @param inst
 * @param line
 */
void instExtractor(instance *inst, char *line);

/**
 * @brief 
 * 
 * @param inst
 * @param filename
 * 
 * @return 
 */
int manageLauncher(instance *inst, const char *filename);

/**
 * @brief 
 * 
 * @param inst
 * 
 * @return
 */
int execute_workflow(instance *inst);

/**
 * @brief 
 * 
 * @param inst
 * @param csv_path
 * 
 * @return 
 */
int runPythonScript(instance *inst, char* csv_path);

#endif /* LAUNCHER_H */