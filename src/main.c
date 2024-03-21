// MacOS
// rm -r build && mkdir build && cmake -S . -B build
// make -C build && clear && ./TSP_Optimization -file Resource/pr10v1.tsp -model 1

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


int main(int argc, char** argv)
{
    instance inst;
    char AlgorithmName[100];
    char *AlgorithmNamePtr = AlgorithmName;
    
    if(argc<2)
    {
        manage_menu(&inst);
    } else
    {
        parse_args(argc, argv, &inst);
    }
    exit(1);

    read_input(&inst);

    // -----------------------------------------------------
    // Fix all the previous function befor continue with the refactor

    apply_algorithm(&inst, AlgorithmName);

    show_solution(&inst, true);
    save_solution(&inst, AlgorithmName);

    free_instance(&inst);
    return 0;
}

