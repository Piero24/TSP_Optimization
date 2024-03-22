// MacOS
// rm -r build && mkdir build && cmake -S . -B build
// make -C build && clear && ./TSP_Optimization -file Resource/pr10v1.tsp -model 1 -opt 1 -v 50 

// Windows
// rmdir /S /Q build && mkdir build && cmake . -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -B build
// make -C build && cls && TSP_Optimization -file Resource/pr10v1.tsp -model 1

#include <stdio.h>
#include <time.h>

#include "../include/tsp.h"
#include "../include/Algorithm/NN.h"
#include "../include/parser.h"
#include "../include/algoSelector.h"
#include "../include/menu.h"


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
// ***
// ********************************************************************************************************************* 
// *********************************************************************************************************************

int main(int argc, char** argv)
{
    instance inst;
    
    if(argc<2)
    {
        manage_menu(&inst);
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

