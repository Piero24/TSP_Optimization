#include "vrp.h"
#include <stdio.h>

int apply_algorithm(instance* inst);

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

int apply_algorithm(instance* inst)
{
    return 0;
}