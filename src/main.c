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

    apply_algorithm(&inst);

    print_solution(&inst, true);

    free_instance(&inst);

    return 0;
}

int apply_algorithm(instance* inst)
{
    int bestFirst = 0;
    double bestCost = -1;

    for(int firstNode=0; firstNode<inst->nnodes; firstNode++){
        int* result;
        double cost;
        result = (int *) calloc(inst->nnodes, sizeof(int));

        nearestNeighbor(result, &cost, inst, firstNode);
        
        printf("\nfirstNode:%d cost: %f\n", firstNode, cost);
        for(int i=0; i<inst->nnodes; i++){
            printf("%d ", result[i]);
        }

        if(bestCost == -1 || bestCost > cost){
            bestCost = cost;
            bestFirst = firstNode;

            free(inst->best_sol);
            inst->best_sol = result;
        }else{
            free(result);
        }
    }

    printf("\nbestNode:%d bestCost: %f\n", bestFirst, bestCost);
    inst->zbest = bestCost;
    
    return 0;
}