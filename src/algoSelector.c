#include "../include/algoSelector.h"

int apply_algorithm(instance* inst)
{
    algorithmSelector(inst);

    if(inst->verbose > 0)
    {
        double time =((double) (inst->tbest - inst->tstart)) / CLOCKS_PER_SEC;
        printf("\n\n");

        char info[] = " BEST SOLUTION ";
        printCentered(info, '*');
	    printf("\n");

        printf("\nSelected Algorithm: %s\t\t\tStarting Node: %d\t\t\tTime: %f sec.\t\t\tCost: %f\t\t\n", inst->algorithm_name, inst->start, time, inst->zbest);
        printf("\n");

        printHorizontalLine('*');
    }

    return 0;
}

int algorithmSelector(instance* inst)
{
    // If statement for selecting the algorithm 
    if (strcmp(inst->algorithm_name, "Nearest Neighbor") == 0)
    {
        NNFromEachNode(inst);

    } else if (strcmp(inst->algorithm_name, "Variable Neighborhood Search") == 0)
    {
        printf("Model type not implemented\n");
        exit(0);

    } else if (strcmp(inst->algorithm_name, "Algo 3") == 0)
    {
        printf("Model type not implemented\n");
        exit(0);

    } else 
    {
        printf("Model type not implemented\n");
        exit(0);

    } 

    show_solution(inst, true);

    if (strcmp(inst->opt_name, "None") != 0) {
        
        twoOpt(inst);
        
        // If statement for selecting the optimization method
        if (strcmp(inst->opt_name, "Tabu Search") == 0)
        {
            tabuSearch(inst);
            
        } else if (strcmp(inst->opt_name, "Opt 3") == 0)
        {
            printf("Optimization method not implemented\n");
            exit(0);

        }
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