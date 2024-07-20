#include "Algorithm/Optimizators/vns.h"

int variableNeighborhoodSearch(instance* inst)
{
    // time checkers
    void* current_time = currentTime();
    struct timespec end = *((struct timespec*)current_time);
    free(current_time);
	
    double time = timeElapsed(&(inst->tstart), &(end));

    //result vector initialization
    verbose_print(inst, 80, "[VNS] Starting initialization.\n");
    
    int* result = (int*) calloc(inst->nnodes, sizeof(int));
    double cost = inst->zbest;
    for (int i = 0; i < inst->nnodes; i++)
        result[i] = inst->best_sol[i];
    
    inst->plotCosts = NULL;
    point* costs = (point*)calloc(1, sizeof(point));
    int nCosts = 0, xIndex = 0;

    verbose_print(inst, 80, "[VNS] Initialization completed, starting optimization.\n");

    // cicle 2opt + kick
    while(time < inst->time_limit){
            
        // 2OPT SECTION

        if(twoOptLoop(inst, result, &cost, costs, &nCosts, &xIndex, true, false, false) != 0)
            break;

        verbose_print(inst, 80, "[VNS - 2opt] Optimization completed, cost: %f, kicking the solution.\n", cost);

        // KICKS SECTION
        int nkicks = (rand() % 9) + 2; // range 2-10
        for(;nkicks > 0; nkicks--){
            kick(&cost, result, inst);
        }

        xIndex++;
        if(inst->plotFlag){
            costs[nCosts].x = xIndex;
            costs[nCosts].y = cost;

            nCosts++;
            show_costs(inst, costs, nCosts, false);

            point* tmp = (point*)calloc(nCosts+1, sizeof(point));
            for(int i=0;i<nCosts;i++){
                tmp[i] = costs[i];
            }

            free(costs);
            costs = tmp;
        }

        verbose_print(inst, 80, "[VNS - 2opt] Kicks done, cost: %f.\n\n", cost);

        // TIME CHECK
        current_time = currentTime();
        end = *((struct timespec*)current_time);
        free(current_time);
        time = timeElapsed(&(inst->tstart), &(end));

        verbose_print(inst, 90, "[VNS] time: %f, limit:%f\n", time, inst->time_limit);

    }
    
    free(result);
    
    return 0;
}
