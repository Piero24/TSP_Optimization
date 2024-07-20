#include "Algorithm/Optimizators/twoOpt.h"

int twoOptLoop(instance* inst, int* result, double* cost, point* costs, int* nCosts, int* xIndex, bool VNS, bool plotFlag, bool mipstart)
{
    int counter = 0; // conts from how many cicles we haven't swapped two nodes

    do 
    {
        // check time limit
        clock_t end = clock();
        double time;// = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;
        if (time >= inst->time_limit) {
            verbose_print(inst, 90, "[2optLoop] Exiting by time limit\n");
            break;
        }

        counter++;
        *xIndex++;

        int* A = &result[0];
        int* A1 = &result[1];

        for (int* B = &result[2]; B < &result[inst->nnodes-2]; B++)
        {
            int* B1 = B + 1;
            
            // Calculate the euclidean distance between the nodes: d(A1,B1) + d(A,B) < d(A1,A) + d(B1,B)
            double distA1B1 = dist(inst, *A1, *B1);
            double distAB = dist(inst, *A, *B);

            double distAA1 = dist(inst, *A, *A1);
            double distBB1 = dist(inst, *B, *B1);

            verbose_print(inst, 100, "[2optLoop] A: %f - %f\n", inst->coord[*A].x, inst->coord[*A].y);
            verbose_print(inst, 100, "[2optLoop] A1: %f - %f\n", inst->coord[*A1].x, inst->coord[*A1].y);
            verbose_print(inst, 100, "[2optLoop] B: %f - %f\n", inst->coord[*B].x, inst->coord[*B].y);
            verbose_print(inst, 100, "[2optLoop] B1: %f - %f\n", inst->coord[*B1].x, inst->coord[*B1].y);
            verbose_print(inst, 100, "[2optLoop] Trying to swap edges %d-%d and %d-%d with %d-%d and %d-%d\n", *A1, *A, *B, *B1,*A1, *B1, *B, *A);
            verbose_print(inst, 100, "[2optLoop] Distances: A1B1: %f, AB: %f, AA1: %f, BB1: %f\n\n", distA1B1, distAB, distAA1, distBB1);

            if (distAB + distA1B1 < distAA1 + distBB1)
            {
                verbose_print(inst, 95, "[2optLoop] Swapping edges %d-%d and %d-%d with %d-%d and %d-%d\n", *A, *A1, *B, *B1, *A1, *B1, *B, *A);

                // Reverse the sub-array from A to B
                reverseSubvector(A1, B);

                *cost = *cost - (distAA1 + distBB1) + (distAB + distA1B1);
                counter = 0;

                // update official solution
                if(*cost < inst->zbest && !mipstart)
                {
                    if(bestSolution(result, *cost, inst) != 0 && VNS)
                    {
                        verbose_print(inst, 60, "[2optLoop] Optimization NOT completed, time limit reached.\n\n");
                        free(result);
                        return 1;
                    }
                }

                if(plotFlag)
                {
                    costs[*nCosts].x = *xIndex;
                    costs[*nCosts].y = *cost;

                    *nCosts++;
                    if(!VNS)
                        show_costs(inst, costs, *nCosts, false);

                    point* tmp = (point*)calloc(*nCosts+1, sizeof(point));
                    for(int i=0;i<*nCosts;i++)
                    {
                        tmp[i] = costs[i];
                    }

                    free(costs);
                    costs = tmp;
                }
            }
        }
        
        // move A to the end of the array
        int Acopy = *A;
        for(int i=0; i<inst->nnodes-1; i++)
        {
            result[i] = result[i+1];
        }

        result[inst->nnodes-1] = Acopy;

    } while (counter < inst->nnodes);

    if(inst->debug){
        for(int j=0; j<inst->nnodes; j++) 
        {
            int* A = &result[0];

            //check that there aren't doable swaps
            for (int* B = &result[2]; B < &result[inst->nnodes-2]; B++)
                assert(dist(inst, *A, *B) + dist(inst, *(A+1), *(B+1)) >= dist(inst, *A, *(A+1)) + dist(inst, *B, *(B+1)));
            
            // move A to the end of the array
            int Acopy = *A;
            for(int i=0; i<inst->nnodes-1; i++)
                result[i] = result[i+1];
            result[inst->nnodes-1] = Acopy;
        }
    }

    return 0;
}

int twoOpt(instance* inst)
{
    verbose_print(inst, 80, "[2opt] Starting initialization.\n");

    int* result = (int*) calloc(inst->nnodes, sizeof(int));
    double cost = inst->zbest;

    for (int i = 0; i < inst->nnodes; i++)
    {
        result[i] = inst->best_sol[i];
    }
    
    inst->plotCosts = NULL;
    point* costs = (point*)calloc(1, sizeof(point));
    int nCosts = 0, xIndex = 0;
    bool plotFlag = false;

    verbose_print(inst, 80, "[2opt] Initialization completed, starting optimization.\n");

    twoOptLoop(inst, result, &cost, costs, &nCosts, &xIndex, false, false, false);

    verbose_print(inst, 80, "[2opt] Optimization completed.\n\n");

    bestSolution(result, cost, inst);
    free(result);
    free(costs);
    
    return 0;
}