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

