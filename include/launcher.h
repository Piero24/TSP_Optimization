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


#define MAX_FILES 100
#define MAX_HEADER_LENGTH 350
#define MAX_PARAMETERS_LENGTH 256

typedef struct {
	char algorithm[50];
    char optimizer[50];
    bool mipstart;
	bool callback_base;
	bool callback_relax; 
	bool posting_base;
	bool posting_relax;
} Tuple;

/**
 * @brief Extracts instructions from a given line and stores them in the instance.
 * 
 * This function takes a line of text as input, tokenizes it, and then parses the tokens
 * to extract instructions, which are then stored in the provided instance.
 * 
 * @param inst The instance to store the extracted instructions in.
 * @param line The line of text to extract instructions from.
 */
void instExtractor(instance *inst, char *line);

/**
 * @brief Generates a header string based on the provided tuples and tuple count.
 * 
 * This function takes a tuple count and an array of tuples as input, and generates
 * a header string that includes the algorithm and optimizer names from the tuples.
 * The generated header string is stored in the provided header_and_lines buffer.
 * 
 * @param header_and_lines The buffer to store the generated header string in.
 * @param tuple_count The number of tuples to process.
 * @param tuples The array of tuples to extract algorithm and optimizer names from.
 */
void headerGenerator(char *header_and_lines, int tuple_count, Tuple *tuples);

/**
 * @brief Generates a CSV file path based on the current date and time.
 * 
 * This function takes a buffer to store the generated CSV file path as input.
 * It uses the current date and time to generate a unique file name in the format
 * "YY-MM-DD_HH-MM.csv" and stores it in the provided buffer. The function also
 * creates the necessary directory structure if it does not already exist.
 * 
 * @param csv_path The buffer to store the generated CSV file path in.
 */
void csvGenerator(char *csv_path);

/**
 * @brief Manages the launcher workflow, extracting instances, 
 * generating CSV files, and executing the workflow.
 * 
 * This function takes an instance and a filename as input, and manages 
 * the entire launcher workflow. It reads the input file, extracts instances, 
 * generates a CSV file, and executes the workflow for each instance.
 * 
 * @param inst The instance to manage the workflow for.
 * @param filename The input file to read instances from.
 * 
 * @return 0 on success, 1 on error.
 */
int manageLauncher(instance *inst, const char *filename);

/**
 * @brief Executes the workflow for a given instance.
 * 
 * This function takes an instance as input and executes the 
 * entire workflow for it. The workflow consists of printing the instance 
 * parameters, reading the input, applying the algorithm,
 * and saving the solution.
 * 
 * @param inst The instance to execute the workflow for.
 * 
 * @return 0 on success.
 */
int executeWorkflow(instance *inst);

/**
 * @brief Runs a Python script to generate a performance profile PDF.
 * 
 * This function takes an instance and a CSV file path as input, and runs a Python script
 * to generate a performance profile PDF using the provided CSV file.
 * The script is executed only if a virtual environment is active.
 * 
 * @param inst The instance associated with the CSV file.
 * @param csv_path The path to the CSV file to use as input for the Python script.
 * 
 * @return 0 on success, 1 on error.
 */
int runPythonScript(instance *inst, char* csv_path);

#endif /* LAUNCHER_H */