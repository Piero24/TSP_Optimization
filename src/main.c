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


int main(int argc, char** argv)
{
    if(argc<2)
    {
        printf("Usage: %s -help for help\n", argv[0]);
        return 1;
    }

    instance inst;
    char AlgorithmName[100];
    char *AlgorithmNamePtr = AlgorithmName;

    parse_args(argc, argv, &inst);
    read_input(&inst);

    for(int i=0;i<inst.nnodes;i++){
        printf("Coord of %d: %f-%f", i, inst.coord[i].x, inst.coord[i].y);
    }

    apply_algorithm(&inst, AlgorithmName);

    show_solution(&inst, true);
    save_solution(&inst, AlgorithmName);

    free_instance(&inst);
    return 0;
}

