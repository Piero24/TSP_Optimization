#include "../../include/Algorithm/optimizations.h"


/**
 * @brief Optimizes a solution by removing intersections
 * * Check every quartet of nodes A, B, C and D where D is the predecessor of A,
 * * C is the successor of B and there is a path starting from A to B that does not 
 * * go through C and D.
 *  
 * * visualization:   | - | - | D | A | - | - | - | B | C | - | - | - |
 * *         path between A and B-->  ^^^^^^^^^^^^^
 * 
 * * We try to see if swapping A-D with A-C and B-C with B-D would reduce the cost.
 * * We can achieve this by using the formula for the euclidean distance d().
 * * By applying it to the pats d(A,C) + d(B,D) < d(A,D) + d(B,C) we have that if
 * * d(A,C) + d(B,D) is less then d(A,D) + d(B,C) the new path is better
 * * then the old one and we have to swap all the inner node (sub-array from A to B).
 * 
 * * from:            | - | - | D | A | x | y | z | B | C | - | - | - |
 * * to:              | - | - | D | B | z | y | x | A | C | - | - | - |
 * 
 * * When there is no more swap to do the new path found is the final path we must return.
 * 
 * @param result Pointer to the array with the inizial solution that will be optimized. 
 *  Will contain the optimized solution after the function ends.
 * @param cost Pointer to the variable to store the cost of the solution.
 * @param inst Pointer to the instance structure.
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int optimization(instance* inst)
{
    if (inst->verbose > 80) printf("[2opt] Starting initialization.\n");

    int* result = (int*) calloc(inst->nnodes, sizeof(int));
    double cost = inst->zbest;
    int flag = 0;
    int A = 0;
    
    for (int i = 0; i < inst->nnodes; i++)
    {
        result[i] = inst->best_sol[i];
    }
    
    if (inst->verbose > 80) printf("[2opt] Initialization completed, starting optimization.\n");

    do 
    {
        flag += 1;

        for (int B = A + 1; B < inst->nnodes; B++)
        {
            int D, C;
            if (A == 0)
            {
                D = inst->nnodes - 1;
            } else 
            {
                D = A - 1;
            }

            if (B == inst->nnodes - 1) 
            {
                C = 0;
            } else 
            {
                C = B + 1;
            }

            // Calculate the euclidean distance between the nodes: d(A,C) + d(B,D) < d(A,D) + d(B,C)
            double distAC = inst->distances[result[A]][result[C]];
            double distBD = inst->distances[result[B]][result[D]];

            double distAD = inst->distances[result[A]][result[D]];
            double distBC = inst->distances[result[B]][result[C]];

            if (inst->verbose > 95){
                printf("[2opt] Trying to swap edges %d-%d and %d-%d with %d-%d and %d-%d\n", result[A], result[D], result[B], result[C], result[A], result[C], result[B], result[D]);
                printf("[2opt] Distances: AC: %f, BD: %f, AD: %f, BC %f\n\n", distAC, distBD, distAD, distBC);

                //9 8, 4 3
                if((result[A] == 9 || result[A] == 8 || result[A] == 4 || result[A] == 3) &&
                    (result[B] == 9 || result[B] == 8 || result[B] == 4 || result[B] == 3) &&
                    (result[C] == 9 || result[C] == 8 || result[C] == 4 || result[C] == 3) &&
                    (result[D] == 9 || result[D] == 8 || result[D] == 4 || result[D] == 3)){
                    printf("[2opt] WARNING: INTERESTING NODES\n\n");
                }
            }

            if (distAC + distBD < distAD + distBC)
            {
                if (inst->verbose > 95) 
                    printf("[2opt] Swapping edges %d-%d and %d-%d with %d-%d and %d-%d\n", result[A], result[D], result[B], result[C], result[A], result[C], result[B], result[D]);

                // Reverse the sub-array from A to B
                int i = A;
                int j = B;

                while (i < j)
                {
                    int tmp = result[i];
                    result[i] = result[j];
                    result[j] = tmp;
                    i++;
                    j--;
                }

                cost = cost - (distAD + distBC) + (distAC + distBD);
                flag = 0;

                // update official solution
                bestSolution(result, cost, inst);
            }
        }
        
        A++;

        // sposta D alla fine

        // A deve essere D
    
    } while (flag < inst->nnodes);

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