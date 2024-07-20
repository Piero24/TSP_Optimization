
// ********************************************************************************************************************* 
// ********************************************************************************************************************* 
// ***
// ***                                                     COMMANDS
// ***
// *** MacOS
// *** rm -r build && mkdir build && cmake -S . -DCPLEXDIR="/Applications/CPLEX_Studio2211/cplex" -B build
// *** rm -r build && mkdir build && cmake -S . -B build
// *** make -C build && clear && ./TSP_Optimization -file Resource/pr10v1.tsp -model 1 -opt 1 -v 50
// *** make -C build && clear && ./TSP_Optimization -launcher Resource/Launcher/launcher.txt
// *** Resource/Launcher/launcher.txt
// ***
// *** Windows
// *** cmake . -B build --fresh && cmake --build build --clean-first
// *** Debug\TSP_Optimization -g 300 -model 5 -v 60 -tl 60 -s 0
// *** Debug\TSP_Optimization -f Resource/a280.tsp -model 3 -v 60
// *** Debug\TSP_Optimization -launcher Resource/Launcher/launcher.txt
// ***
// *** Linux
// *** rm -r build && mkdir build && cmake -S . -DCPLEXDIR="C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/cplex/" -B build
// *** make -C build && clear && ./TSP_Optimization
// ***
// *** CPLEX OPTIONS: 1:mipstart, 2:callback base, 3:callback relax, 4:posting base, 5:posting relax
// ***
// ********************************************************************************************************************* 
// *********************************************************************************************************************

#include <stdio.h>
#include <time.h>

#include "tsp.h"
#include "Algorithm/Heuristics/NN.h"
#include "parser.h"
#include "algoSelector.h"
#include "menu.h"
#include "launcher.h"

int main(int argc, char** argv)
{

    // calculate the time to do this code
    clock_t start = clock();
    sleep_ms(1000);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent: %f\n", time_spent);
    return 0;


    instance inst;
    inst.debug = false;
    
    if(argc<2)
    {
        manageMenu(&inst);

    } else if (argc == 3 && strcmp(argv[1], "-launcher") == 0)
    {
        manageLauncher(&inst, argv[2]);
        return 0;

    } else
    {
        parse_args(argc, argv, &inst);
    }

    executeWorkflow(&inst);
    free_instance(&inst);

    //while(true){}

    return 0;
}