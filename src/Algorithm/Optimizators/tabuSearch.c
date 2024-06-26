#include "Algorithm/Optimizators/tabuSearch.h"

int tabuSearch(instance* inst)
{
    verbose_print(inst, 80, "[Tabu' Search] Starting initialization.\n");

    // Copying result vector
    int* result = (int*) calloc(inst->nnodes, sizeof(int));
    double cost = inst->zbest;

    for (int i = 0; i < inst->nnodes; i++)
    {
        result[i] = inst->best_sol[i];
    }

    // tabù list, tenure and tabuList current size
    int tenure = (inst->nnodes / 10 > 10) ? inst->nnodes / 10 : inst->nnodes / 2;
    int* tabuList = (int *) calloc(tenure + 1, sizeof(int));
    int tabuPos = 0;

    for (int i = 0; i < tenure; i++)
        tabuList[i] = -1;

    inst->plotCosts = NULL;
    point* costs = (point*)calloc(1, sizeof(point));
    int nCosts = 0, xIndex = 0;
    bool plotFlag = false;

    // time checkers
    clock_t end = clock();
    double time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;
    
    verbose_print(inst, 80, "[Tabu' Search] Initialization completed, starting optimization.\n");

    while(time < inst->time_limit)
    {
        // INITIALIZATION:
        /*
        * We will search the best path among a neighborhood of the current solution (result[]).
        * At the beginning we put a negative cost path as best path candidate, it will be updated
        * as soon as the HEURISTIC SEARCH will find the first feasible solution.
        */

        // best candidate and best candidate cost
        int* bestSol = (int *) calloc(inst->nnodes, sizeof(int));
        double bestCost = -1;
        int swappedNode = -1;
        xIndex++;

        // HEURISTIC SEARCH:
        /*
        * The neighborhood of the current solution consists of all the solutions that have 2 
        * swapped nodes wrt the current solution.
        * We try to swap each couple of nodes and we 
        */

        // swap two nodes A and B
        for(int A = 0; A < inst->nnodes; A++)
        {
            // if node A is in tabù list, skip it
            if(nodeInList(&result[A], tabuList, tenure)) continue;
            
            // choose node B to swap
            for(int B = A+1; B < inst->nnodes; B++)
            {
                // if node B is in tabù list, skip it
                if(nodeInList(&result[B], tabuList, tenure) || (A == 0 && B == inst->nnodes-1)) continue;
                
                // save indexes of node neighbors
                int prevA = (A == 0) ? inst->nnodes-1 : A-1;
                int succB = (B == inst->nnodes-1) ? 0 : B+1;

                //compute costs
                double old = dist(inst, result[prevA], result[A]) + dist(inst, result[succB], result[B]);
                double new = dist(inst, result[prevA], result[B]) + dist(inst, result[succB], result[A]);
                
                // if is the first non-tabù solution, save it as current candidate
                if(bestCost == -1 || new < old)
                {
                    if (bestCost == -1)
                        verbose_print(inst, 80, "[Tabu' Search] First feasible solution found, saving it\n");
                    
                    // copy current solution
                    copyArray(result, bestSol, inst->nnodes);

                    // do swap and update costs
                    reverseSubvector(&bestSol[A], &bestSol[B]);
                    bestCost = cost - old + new;

                    // save swapped node
                    swappedNode = result[A];

                    verbose_print(inst, 90, "[Tabu' Search] Swapping nodes %d and %d. New cost is %f\n", result[A], result[B], bestCost);
                }
            }
        }
        
        // UPDATE TABÙ LIST
        /*
         * If the best solution in the neighborhood is worse than the current solution, we
         * save a swapped node in the tabù list to avoid going back.
         * Otherwise we can update the current best solution.
         */
        
        if(bestCost >= cost)
        {
            tabuList[tabuPos] = swappedNode;
            tabuPos++;

            // if tabù list is full do FIFO
            if(tabuPos > tenure)
                tabuPos = 0;

            verbose_print(inst, 80, "[Tabu' Search] The new solution is worse, adding it to tabu' list\n");
        }

        verbose_print(inst, 80, "[Tabu' Search] Updating new 'current' solution. Old cost: %f, New cost: %f\n", cost, bestCost);
        
        copyArray(bestSol, result, inst->nnodes);
        cost = bestCost;

        if(cost < inst->zbest)
        {
            verbose_print(inst, 80, "[Tabu' Search] Updating best official solution.\n");
        
            // update official solution
            bestSolution(result, cost, inst);
        }

        free(bestSol);

        if(plotFlag){
            costs[nCosts].x = xIndex;
            costs[nCosts].y = bestCost;

            nCosts++;
            show_costs(inst, costs, nCosts, false);

            point* tmp = (point*)calloc(nCosts+1, sizeof(point));
            for(int i=0;i<nCosts;i++){
                tmp[i] = costs[i];
            }

            free(costs);
            costs = tmp;
        }
        
        // TIME CHECK
        /*
         * The algorithm stops when the it has ran for too much time (more than the time limit).
         * Here we check if this has happened.
         */

        end = clock();
        time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;

        verbose_print(inst, 80, "[Tabu' Search] time: %f, limit:%f\n", time, inst->time_limit);

        //system("pause");

    }

    verbose_print(inst, 80, "[Tabu' Search] Optimization completed.\n\n");

    free(result);
    free(tabuList);
    free(costs);

    return 0;
}