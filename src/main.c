
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
// *** Debug\TSP_Optimization.exe -g 100 -model 1 -opt 3 -s 0 -v 60 -tl 10
// *** for /L %i in (1,1,10) do @echo. & if %i==10 (Debug\TSP_Optimization.exe -g 100 -model 3 -v 95)
// *** Debug\TSP_Optimization.exe -g 100 -model 3 -v 60
// *** Debug\TSP_Optimization.exe -f Resource/a280.tsp -model 3 -v 60
// ***
// *** Linux
// *** rm -r build && mkdir build && cmake -S . -DCPLEXDIR="C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2211/cplex/" -B build
// *** make -C build && clear && ./TSP_Optimization
// ***
// ********************************************************************************************************************* 
// *********************************************************************************************************************

#include <stdio.h>
#include <time.h>

#include "tsp.h"
#include "Algorithm/NN.h"
#include "parser.h"
#include "algoSelector.h"
#include "menu.h"
#include "launcher.h"

int main(int argc, char** argv)
{
    instance inst;
    
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
    return 0;
}