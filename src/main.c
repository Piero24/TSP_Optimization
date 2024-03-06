// clear && gcc -o main *.c && ./main -file FILE-NAME-HERE
// clear && gcc -o main *.c && ./main -file pr1002.tsp

#include "vrp.h"
#include "NN.h"
#include "parser.h"
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

    for(int i=0; i<inst.nnodes; i++){
        for(int j=0; j<inst.nnodes; j++){
            printf("%f ", inst.distances[i][j]);
        }
        printf("\n");
    }

    apply_algorithm(&inst);

    //print_solution(&inst, true);

    free_instance(&inst);

    return 0;
}

int apply_algorithm(instance* inst)
{
    int* result;
    result = (int *) calloc(inst->nnodes, sizeof(int));

    nearestNeighbor(result, inst, 0);
    
    for(int i=0; i<inst->nnodes; i++){
        printf("%d ", result[i]);
    }

    free(result);
    return 0;
}