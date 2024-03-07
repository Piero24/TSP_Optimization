// rm -r build && mkdir build && cmake -S . -B build
// make -C build && ./TSP_Optimization -file Resource/pr10.tsp

// 
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

    parse_args(argc, argv, &inst);

    read_input(&inst);

    apply_algorithm(&inst);

    print_solution(&inst, true);

    free_instance(&inst);

    return 0;
}

