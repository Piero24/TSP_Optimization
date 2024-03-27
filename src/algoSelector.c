#include "algoSelector.h"

void Random(instance* inst)
{
    for (int i = 0; i < inst->nnodes; i++)
    {
        inst->best_sol[i] = i;
    }

    for (int i = 0; i < inst->nnodes/4; i++)
    {
        int a = rand() % (inst->nnodes + 1);
        int b = rand() % (inst->nnodes + 1);

        int temp = inst->best_sol[a];
        inst->best_sol[a] = inst->best_sol[b];
        inst->best_sol[b] = temp;
    }

    inst->zbest = 0;
    for (int i = 1; i < inst->nnodes; i++)
    {
        inst->zbest += inst->distances[inst->best_sol[i]][inst->best_sol[i-1]];
    }
    inst->zbest += inst->distances[inst->best_sol[0]][inst->best_sol[inst->nnodes-1]];
}

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
    if (strcmp(inst->algorithm_name, "Random") == 0)
    {
        Random(inst);

    } else if (strcmp(inst->algorithm_name, "Nearest Neighbor") == 0)
    {
        NNFromEachNode(inst);

    } else if (strcmp(inst->algorithm_name, "Algo 3") == 0)
    {
        printf("Model type not implemented\n");
        exit(0);

    } else 
    {
        printf("Model type not implemented\n");
        exit(0);
    } 

    // If statement for selecting the optimization method
    if (strcmp(inst->opt_name, "2-Opt") == 0){
        twoOpt(inst);
    
    }else if (strcmp(inst->opt_name, "Tabu Search") == 0)
    {
        twoOpt(inst);
        tabuSearch(inst);
        
    }else if (strcmp(inst->opt_name, "Variable Neighborhood Search") == 0)
    {
        variableNeighborhoodSearch(inst);

    }else if (strcmp(inst->opt_name, "Opt 3") == 0)
    {
        printf("Optimization method not implemented\n");
        exit(0);

    }
    return 0;
}

void sleep_ms(int milliseconds)
{
    #ifdef WIN32
        Sleep(milliseconds);
    #elif _POSIX_C_SOURCE >= 199309L
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000000;
        nanosleep(&ts, NULL);
    #else
        usleep(milliseconds * 1000);
    #endif
}

int bestSolution(int* result, double cost, instance* inst)
{
    for (int i = 0; i < inst->nnodes; i++)
    {
        inst->best_sol[i] = result[i];
    }
    inst->zbest = cost;
    inst->tbest = clock();

    int time = (int)(((double) (inst->tbest - inst->tstart)) / CLOCKS_PER_SEC);
    
    if(inst->verbose >= 50)
        printf("Best solution updated. Its cost is %f, it was founded after %d seconds\n", cost, time);
    
    if(inst->show_gnuplot > -1){
        if(inst->show_gnuplot > 0)
            sleep_ms(inst->show_gnuplot*1000);
        show_solution(inst, true);
    }

    if (time >= inst->time_limit)
        return 1;
    return 0;
}