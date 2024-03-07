#include <stdio.h>
#include <time.h>

#include "../include/tsp.h"
#include "../include/Algorithm/NN.h"
#include "../include/parser.h"
#include "../include/algoSelector.h"

int apply_algorithm(instance* inst, bool onlyBestSolution)
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
        
        algorithmSelector(result, &cost, inst, firstNode);

        end = clock();
        time = ((double) (end - start)) / CLOCKS_PER_SEC;

        if (!onlyBestSolution)
        {
            printf("\nStarting Node: %d\tTime: %f sec.\tCost: %f\n", firstNode, time, cost);
            printf("Output Path -> [");
            for(int i=0; i<inst->nnodes; i++)
            {
                if (i<inst->nnodes-1) 
                {
                    printf("%d, ", result[i]);
                } else
                {
                    printf("%d", result[i]);
                }
            }
            printf("]\n\n");
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

    printf("\n\n\n************************ BEST SOLUTION ************************\n");
    printf("\nStarting Node: %d\tTime: %f sec.\tCost: %f\n", bestFirst, inst->tbest, bestCost);
    printf("\n***************************************************************\n\n\n");

    inst->zbest = bestCost;
    return 0;
}

int algorithmSelector(int* result, double* cost, instance* inst, int firstNode)
{

    if (inst->model_type == 1)
    {
        nearestNeighbor(result, cost, inst, firstNode);

    } else if (inst->model_type == 2)
    {
        printf("Model type not implemented\n");
        exit(0);

    } else if (inst->model_type == 3)
    {
        printf("Model type not implemented\n");
        exit(0);

    } else if (inst->model_type == 4)
    {
        printf("Model type not implemented\n");
        exit(0);

    } else if (inst->model_type == 5)
    {
        printf("Model type not implemented\n");
        exit(0);

    } else 
    {
        printf("Model type not implemented\n");
        exit(0);

    } 
    return 0;
}