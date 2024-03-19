#include "../../include/Algorithm/optimizations.h"


int twoOpt(instance* inst)
{
    if (inst->verbose > 80) printf("[2opt] Starting initialization.\n");

    int* result = (int*) calloc(inst->nnodes, sizeof(int));
    double cost = inst->zbest;
    int counter = 0; // conts from how many cicles we haven't swapped two nodes
    
    for (int i = 0; i < inst->nnodes; i++)
    {
        result[i] = inst->best_sol[i];
    }
    
    if (inst->verbose > 80) printf("[2opt] Initialization completed, starting optimization.\n");

    do 
    {
        counter += 1;

        int* A = &result[0];
        int* A1 = &result[1];

        for (int* B = &result[2]; B < &result[inst->nnodes-2]; B++)
        {
            int* B1 = B + 1;
            
            // Calculate the euclidean distance between the nodes: d(A1,B1) + d(A,B) < d(A1,A) + d(B1,B)
            double distA1B1 = inst->distances[*A1][*B1];
            double distAB = inst->distances[*A][*B];

            double distAA1 = inst->distances[*A][*A1];
            double distBB1 = inst->distances[*B][*B1];

            if (inst->verbose > 95){
                printf("[2opt] A: %f - %f\n", inst->coord[*A].x, inst->coord[*A].y);
                printf("[2opt] A1: %f - %f\n", inst->coord[*A1].x, inst->coord[*A1].y);
                printf("[2opt] B: %f - %f\n", inst->coord[*B].x, inst->coord[*B].y);
                printf("[2opt] B1: %f - %f\n", inst->coord[*B1].x, inst->coord[*B1].y);
                printf("[2opt] Trying to swap edges %d-%d and %d-%d with %d-%d and %d-%d\n", *A1, *A, *B, *B1,*A1, *B1, *B, *A);
                printf("[2opt] Distances: A1B1: %f, AB: %f, AA1: %f, BB1: %f\n\n", distA1B1, distAB, distAA1, distBB1);

                //9 8, 4 3
                if((*A1 == 9 || *A1 == 8 || *A1 == 4 || *A1 == 3) &&
                    (*B == 9 || *B == 8 || *B == 4 || *B == 3) &&
                    (*B1 == 9 || *B1 == 8 || *B1 == 4 || *B1 == 3) &&
                    (*A == 9 || *A == 8 || *A == 4 || *A == 3)){
                    printf("[2opt] WARNING: INTERESTING NODES\n\n");
                }
            }

            if (distAB + distA1B1 < distAA1 + distBB1)
            {
                if (inst->verbose > 95) 
                    printf("[2opt] Swapping edges %d-%d and %d-%d with %d-%d and %d-%d\n", *A, *A1, *B, *B1, *A1, *B1, *B, *A);

                // Reverse the sub-array from A to B
                int* i = A1;
                int* j = B;

                while (i < j)
                {
                    int tmp = *i;
                    *i = *j;
                    *j = tmp;
                    i++;
                    j--;
                }

                cost = cost - (distAA1 + distBB1) + (distAB + distA1B1);
                counter = 0;

                // update official solution
                bestSolution(result, cost, inst);
            }
        }
        
        // move A to the end of the array
        int Acopy = *A;
        for(int i=0; i<inst->nnodes-1; i++){
            result[i] = result[i+1];
        }

        result[inst->nnodes-1] = Acopy;

    } while (counter < inst->nnodes);

    if (inst->verbose > 80) printf("[2opt] Optimization completed.\n\n");

    bestSolution(result, cost, inst);
    
    return 0;
}

int tabuSearch(instance* inst)
{
    if (inst->verbose > 80) printf("[Tabu' Search] Starting initialization.\n");

    // Copying result vector
    int* result = (int*) calloc(inst->nnodes, sizeof(int));
    double cost = inst->zbest;

    for (int i = 0; i < inst->nnodes; i++)
    {
        result[i] = inst->best_sol[i];
    }

    // tabù list, tenure and tabuList current size
    int* tabuList = (int *) calloc(inst->nnodes, sizeof(int));
    int tenure = inst->nnodes / 10;
    int tabuPos = 0;

    // time checkers
    clock_t end;
    double time;
    
    if (inst->verbose > 80) printf("[Tabu' Search] Initialization completed, starting optimization.\n");

    do
    {
        // INITIALIZATION:
        /*
        * We will search the best path among a neighborhood of the current solution (result[]).
        * At the beginning we put an infinite cost path as best path candidate, it will be updated
        * as soon as the HEURISTIC SEARCH will find the first feasible solution.
        */

        // best candidate and best candidate
        int* bestSol = (int *) calloc(inst->nnodes, sizeof(int));
        double bestCost = -1;
        int swappedNode = -1;

        // HEURISTIC SEARCH:
        /*
        * The neighborhood of the current solution consists of all the solutions that have 2 
        * swapped nodes wrt the current solution.
        * We try to swap each couple of nodes and we 
        */

        // swap two nodes A and B
        for(int A = 0; A < inst->nnodes-1; A++)
        {

            // if node A is in tabù list, skip it
            for(int i = 0; i < tabuPos; i++)
                if(result[A] == tabuList[i])
                    continue;
            
            // choose node B to swap
            for(int B = A+1; B < inst->nnodes; B++)
            {
                
                // if node B is in tabù list, skip it
                for(int i = 0; i < tabuPos; i++)
                    if(result[B] == tabuList[i])
                        continue;
                
                // save indexes
                int prevA = (A==0) ? inst->nnodes-1 : A-1;
                int succA = A + 1;
                int prevB = B - 1;
                int succB = (B==inst->nnodes-1) ? 0 : B+1;
                
                // if is the first non-tabù solution, save it as current candidate
                if(bestCost == -1)
                {
                    if (inst->verbose > 95) 
                        printf("[Tabu' Search] First feasible solution found, saving it\n");
                    
                    // reset best solution cost to 0
                    bestCost = 0;

                    // copy current solution and its cost
                    for(int i = 0; i < inst->nnodes; i++)
                    {
                        bestSol[i] = result[i];
                        if(i > 0)
                            bestCost += inst->distances[result[i]][result[i-1]];
                    }

                    // add final cost
                    bestCost += inst->distances[bestSol[0]][bestSol[inst->nnodes-1]];

                    // do swap
                    int tmp = bestSol[A];
                    bestSol[A] = bestSol[B];
                    bestSol[B] = tmp;

                    // add cost of swapped nodes
                    bestCost -= inst->distances[bestSol[prevA]][bestSol[B]];
                    bestCost -= inst->distances[bestSol[succA]][bestSol[B]];
                    bestCost -= inst->distances[bestSol[prevB]][bestSol[A]];
                    bestCost -= inst->distances[bestSol[succB]][bestSol[A]];

                    bestCost += inst->distances[bestSol[prevA]][bestSol[A]];
                    bestCost += inst->distances[bestSol[succA]][bestSol[A]];
                    bestCost += inst->distances[bestSol[prevB]][bestSol[B]];
                    bestCost += inst->distances[bestSol[succB]][bestSol[B]];
                
                } else 
                {
                    // check if cost is better than best candidate
                    int old = inst->distances[bestSol[prevA]][bestSol[A]];
                    old += inst->distances[bestSol[succA]][bestSol[A]];
                    old += inst->distances[bestSol[prevB]][bestSol[B]];
                    old += inst->distances[bestSol[succB]][bestSol[B]];

                    int new = inst->distances[bestSol[prevA]][bestSol[B]];
                    new += inst->distances[bestSol[succA]][bestSol[B]];
                    new += inst->distances[bestSol[prevB]][bestSol[A]];
                    new += inst->distances[bestSol[succB]][bestSol[A]];
                    
                    if(new < old)
                    {
                        
                    
                        // update cost
                        bestCost = bestCost - old + new;

                        // do swap
                        int tmp = bestSol[A];
                        bestSol[A] = bestSol[B];
                        bestSol[B] = tmp;

                        // save swapped node
                        swappedNode = bestSol[A];

                        if (inst->verbose > 95) 
                            printf("[Tabu' Search] Swapping nodes %d and %d. New cost is %f\n", bestSol[A], bestSol[B], bestCost);
                    }
                }
            }
        }

        // UPDATE TABÙ LIST
        /*
         * If the best solution in the neighborhood is worse than the current solution, we
         * save a swapped node in the tabù list to avoid going back.
         * Otherwise we can update the current best solution.
         */
        
        if(bestCost > cost)
        {
            tabuList[tabuPos] = swappedNode;
            tabuPos++;

            // if tabù list is full do FIFO
            if(tabuPos > tenure)
            {
                for(int i=1;i<tabuPos;i++)
                    tabuList[i-1] = tabuList[i];
                tabuPos--;
            }

            if (inst->verbose > 95) 
                printf("[Tabu' Search] The new solution is worse, adding it to tabu' list\n");
        }

        if (inst->verbose > 95) 
            printf("[Tabu' Search] Updating new 'current' solution. Old cost: %f, New cost: %f\n", cost, bestCost);
        
        for(int i = 0; i < inst->nnodes; i++)
        {
            result[i] = bestSol[i];
            cost = bestCost;
        }

        if(cost < inst->zbest)
        {
            // update official solution
            bestSolution(result, cost, inst);
        }

        // TIME CHECK
        /*
         * The algorithm stops when the it has ran for too much time (more than the time limit).
         * Here we check if this has happened.
         */

        end = clock();
        time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;

    }while(time < inst->timelimit);

    if (inst->verbose > 80) printf("[Tabu' Search] Optimization completed.\n\n");

    return 0;
}