#include <stdio.h>
#include <time.h>

#include "../include/tsp.h"
#include "../include/Algorithm/NN.h"
#include "../include/parser.h"

int apply_algorithm(instance* inst)
{
    int bestFirst = 0;
    double bestCost = -1;

    for(int firstNode=0; firstNode<inst->nnodes; firstNode++)
    {
        int* result;
        double cost;
        result = (int *) calloc(inst->nnodes, sizeof(int));

        clock_t start, end;
        double time;
        start = clock();
        
        nearestNeighbor(result, &cost, inst, firstNode);

        end = clock();
        time = ((double) (end - start)) / CLOCKS_PER_SEC;

        printf("\nfirstNode: %d\ttime: %f\tcost: %f\n", firstNode, time, cost);
        for(int i=0; i<inst->nnodes; i++)
        {
            printf("%d ", result[i]);
        }

        if(bestCost == -1 || bestCost > cost)
        {
            bestCost = cost;
            bestFirst = firstNode;

            free(inst->best_sol);
            inst->best_sol = result;
            inst->tbest = time;
        } else
        {
            free(result);
        }
    }

    printf("\nBest:\tfistNode: %d\ttime: %f\tcost: %f\n", bestFirst, inst->tbest, bestCost);
    inst->zbest = bestCost;
    
    return 0;
}