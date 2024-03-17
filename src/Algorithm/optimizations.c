#include <stdio.h>
#include <stdlib.h>

#include "../include/Algorithm/optimizations.h"
#include "../include/Algorithm/NN.h"

int optimization(int* result, double* cost, instance* inst)
{
    int flag = 0;
    int A = 0;
    
    int* copyResult = (int *) calloc(inst->nnodes, sizeof(int));
    
    for (int i = 0; i < inst->nnodes; i++)
    {
        copyResult[i] = result[i];
    }
    
    do 
    {    
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
            double distAC = inst->distances[copyResult[A]][copyResult[C]];
            double distBD = inst->distances[copyResult[B]][copyResult[D]];

            double distAD = inst->distances[copyResult[A]][copyResult[D]];
            double distBC = inst->distances[copyResult[B]][copyResult[C]];

            flag += 1;
            if (distAC + distBD < distAD + distBC)
            {
                // Reverse the sub-array from A to B
                int i = A;
                int j = B;

                while (i < j)
                {
                    int tmp = copyResult[i];
                    copyResult[i] = copyResult[j];
                    copyResult[j] = tmp;
                    i++;
                    j--;
                }

                *cost = *cost - (distAD + distBC) + (distAC + distBD);
                flag = 0;
            }
        }
        
        A++;
    
    } while (flag < inst->nnodes);
    
    for (int i = 0; i < inst->nnodes; i++)
    {
        result[i] = copyResult[i];
    }
    
    return 0;
}

int tabuSearch(int* result, double* cost, instance* inst)
{
    // tabù list, tenure and tabuList current size
    int* tabuList = (int *) calloc(inst->nnodes, sizeof(int));
    int tenure = inst->nnodes / 10;
    int tabuPos = 0;
    
    // time check variables
    clock_t start = clock(), end;
    double cpu_time_used;

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
                    }
                }
            }
        }

        // UPDATE TABÙ LIST
        /*
         * If the best solution in the neighborhood is worse than the current solution, we
         * save a swapped node in the tabù list to avoid going back.
         */
        
        if(bestCost > *cost)
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
        } else 
        {
            for(int i = 0; i < inst->nnodes; i++)
            {
                result[i] = bestSol[i];
                *cost = bestCost;
            }
        }

        // TIME CHECK
        /*
         * The algorithm stops when the it has ran for too much time (more than the time limit).
         * Here we check if this has happened.
         */

        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        
    }while(cpu_time_used < inst->timelimit);

    return 0;
}