#include "../include/algoSelector.h"

int apply_algorithm(instance* inst, char *AlgorithmName)
{
    algorithmSelector(inst, AlgorithmName);

    if(inst->verbose > 0)
    {
        double time =((double) (inst->tbest - inst->tstart)) / CLOCKS_PER_SEC;

        printf("\n\n\n********************************************* BEST SOLUTION *********************************************\n");
        printf("\nSelected Algorithm: %s\tStarting Node: %d\tTime: %f sec.\tCost: %f\n", AlgorithmName, inst->start, time, inst->zbest);
        printf("\n*********************************************************************************************************\n\n\n");
    }

    return 0;
}

int algorithmSelector(instance* inst, char *AlgorithmName)
{
    // // If statement for selecting the algorithm 
    // if (inst->model_type == 1)
    // {
    //     strcpy(AlgorithmName, "Nearest Neighbor");
    //     NNFromEachNode(inst);

    // } else if (inst->model_type == 2)
    // {
    //     printf("Model type not implemented\n");
    //     exit(0);

    // } else if (inst->model_type == 3)
    // {
    //     printf("Model type not implemented\n");
    //     exit(0);

    // } else if (inst->model_type == 4)
    // {
    //     printf("Model type not implemented\n");
    //     exit(0);

    // } else if (inst->model_type == 5)
    // {
    //     printf("Model type not implemented\n");
    //     exit(0);

    // } else 
    // {
    //     printf("Model type not implemented\n");
    //     exit(0);

    // } 

    inst->best_sol[0] = 0;
    for (int i = 1; i < inst->nnodes; i++)
    {
        inst->best_sol[i] = i;
        inst->zbest += inst->distances[i-1][i];
    }

    show_solution(inst, true);

    // If statement for start 2opt 
    if (inst->opt_type > 0)
    {
        optimization(inst);
    }

    show_solution(inst, true);
    
    // If statement for selecting the optimization method
    if (inst->opt_type == 2) 
    {
        tabuSearch(inst);
        
    } else if (inst->model_type == 3)
    {
        printf("Optimization method not implemented\n");
        exit(0);

    }

    return 0;
}

void bestSolution(int* result, double cost, instance* inst)
{
    for (int i = 0; i < inst->nnodes; i++)
    {
        inst->best_sol[i] = result[i];
    }
    inst->zbest = cost;
    inst->tbest = clock();

    if(inst->verbose >= 50)
    {
        int time = (int)(((double) (inst->tbest - inst->tstart)) / CLOCKS_PER_SEC);
        printf("Best solution updated. Its cost is %f, it was founded after %d seconds\n", cost, time);
    }
}