// MacOS
// rm -r build && mkdir build && cmake -S . -DCPLEXDIR="/Applications/CPLEX_Studio2211/cplex" -B build
// make -C build && clear && ./TSP_Optimization -file Resource/pr10v1.tsp -model 1 -opt 1 -v 50
// Resource/Launcher/launcher.txt

// Windows
// cmake . -B build --fresh && cmake --build build --clean-first
// Debug\TSP_Optimization.exe -g 100 -model 1 -opt 3 -s 0 -v 60 -tl 10

// Linux
// rm -r build && mkdir build && cmake -S . -DCPLEXDIR="C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/cplex/" -B build
// make -C build && clear && ./TSP_Optimization

#include <stdio.h>
#include <time.h>

#include "tsp.h"
#include "Algorithm/NN.h"
#include "parser.h"
#include "algoSelector.h"
#include "menu.h"


// ********************************************************************************************************************* 
// ********************************************************************************************************************* 
// ***
// ***                                                     WARNING
// ***
// *** There are a problem with the circular dependency between the parser.c, parser.h, tsp.c tsp.h and menu.c 
// *** menu.h files. There is a temporary solution to this problem, but it is not the best solution.
// ***
// *** For now the import are devided in these files in a way that there is no conflict but if we move the include 
// *** in these files from the .h to the .c file we will have a circular dependency problem.
// ***
// *** The first thing to do is to fix this problem before move forward with the project. Otherwise it will b more
// *** difficult to maintain the code. And find a solution with other functions that will be added in the future.
// ***
// *** The best solution is to find the function/s that are causing the circular dependency and move them to a new file.
// *** I think one of the function that is causing the problem is the char *getFileName(const char *filePath). 
// *** Located in the parser.c file. and used also in other files. But I'm note sure of this conclusion since I didn't
// *** check yet. I suggest to check this function first and than if the problem isn't solved after moving the include
// *** in the headers file check the others func.
// ***
// *** parameterPrint -> menu.c, parser.c
// *** getFileName -> parser.c, tsp.c
// *** parameterPrint -> parser.c, menu.c
// ***
// ********************************************************************************************************************* 
// *********************************************************************************************************************

int manage_launcher(instance *inst, const char *filename);

int main(int argc, char** argv)
{
    instance inst;
    
    if(argc<2)
    {
        manage_menu(&inst);

    } else if (argc == 3 && strcmp(argv[1], "-launcher") == 0)
    {
        manage_launcher(&inst, argv[2]);

    } else
    {
        parse_args(argc, argv, &inst);
    }

    read_input(&inst);
    apply_algorithm(&inst);

    show_solution(&inst, true);
    save_solution(&inst);

    free_instance(&inst);
    return 0;
}

int manage_launcher(instance *inst, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Skip lines starting with "//"
        if (strncmp(line, "//", 2) != 0)
        {
            char *token;
            char *delim = " \n";
            // Assuming a maximum of 256 words per line
            char *words[256];
            // Start with 1 to reserve the first cell for an empty string
            int argc = 1;

            token = strtok(line, delim);
            
            while (token != NULL && argc < 256)
            {
                printf("%s\n", token);
                words[argc++] = token;
                token = strtok(NULL, delim);
            }

            parse_args(argc, words, inst);

            read_input(inst);
            apply_algorithm(inst);

            show_solution(inst, true);
            save_solution(inst);

            free_instance(inst);
        }
    }
    fclose(file);
    return 0;
}//*/