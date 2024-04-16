#include "Algorithm/optimizations.h"

void reverseSubvector(int* from, int* to)
{
    // Reverse the sub-array from A to B
    int* i = from;
    int* j = to;

    while (i < j)
    {
        int tmp = *i;
        *i = *j;
        *j = tmp;
        i++;
        j--;
    }
}

bool nodeInList(int* node, int* list, int listLength)
{
    for(int i = 0; i < listLength; i++)
    {
        if(*node == list[i])
        {
            return true;
        }
    }
    return false;
}

void copyArray(int* from, int* to, int length)
{
    for(int i = 0; i < length; i++)
    {
        to[i] = from[i];
    }
}

/**
    | a | b | c | d | A | A | A | A | A | B | B | B | B | B | o | p | q | r |
	             ^^^ ^^^             ^^^ ^^^             ^^^ ^^^
	              i  i+1     <        j  j+1    <         k  k+1

    i -> j+1
    j+1 -> k
    k -> i+1
    i+1 -> j
    j -> k+1
    k+1 -> i
	
    | a | b | c | d | B | B | B | B | B | A | A | A | A | A | o | p | q | r |
	             ^^^ ^^^             ^^^ ^^^             ^^^ ^^^
	              i  j+1              k  i+1              j  k+1

    code:
    copy array from i+1 to j (included) in tmp
    copy array from j+1 to k (included) in positions from i+1 to (i+1)+(k-(j+1)+1)
    copy tmp in positions from (i+1)+(k-(j+1)+1) to (i+1)+(k-(j+1)+1)+(j-(i+1))
*/
void kick(double* cost, int* result, instance* inst){
    //find 3 points in the vector
    int i = rand() % (inst->nnodes-5);
    int j = (rand() % (inst->nnodes - (i+2) - 3)) + i + 2;
    int k = (rand() % (inst->nnodes - (j+2) - 1)) + j + 2;

    //remove old costs
    *cost -= dist(inst, result[i], result[i+1]);
    *cost -= dist(inst, result[j], result[j+1]);
    *cost -= dist(inst, result[k], result[k+1]);

    //add new costs
    *cost += dist(inst, result[i], result[j+1]);
    *cost += dist(inst, result[j], result[k+1]);
    *cost += dist(inst, result[k], result[i+1]);

    //compute subvectors sizes
    int sizeA = j - (i+1) + 1;
    int sizeB = k - (j+1) + 1;

    //swap subvectors
    int* tmp = (int*) calloc(sizeA, sizeof(int));
    for(int a = 0; a < sizeA; a++){
        tmp[a] = result[i + 1 + a];
    }

    for(int b = 0; b < sizeB; b++){
        result[i + 1 + b] = result[j + 1 + b];
    }

    for(int a = 0; a < sizeA; a++){
        result[i + 1 + sizeB + a] = tmp[a];
    }
}

int twoOptLoop(instance* inst, int* result, double* cost, point* costs, int* nCosts, int* xIndex, bool VNS, bool plotFlag){
    int counter = 0; // conts from how many cicles we haven't swapped two nodes

    do 
    {
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

            if (inst->verbose >= 100){
                printf("[2optLoop] A: %f - %f\n", inst->coord[*A].x, inst->coord[*A].y);
                printf("[2optLoop] A1: %f - %f\n", inst->coord[*A1].x, inst->coord[*A1].y);
                printf("[2optLoop] B: %f - %f\n", inst->coord[*B].x, inst->coord[*B].y);
                printf("[2optLoop] B1: %f - %f\n", inst->coord[*B1].x, inst->coord[*B1].y);
                printf("[2optLoop] Trying to swap edges %d-%d and %d-%d with %d-%d and %d-%d\n", *A1, *A, *B, *B1,*A1, *B1, *B, *A);
                printf("[2optLoop] Distances: A1B1: %f, AB: %f, AA1: %f, BB1: %f\n\n", distA1B1, distAB, distAA1, distBB1);
            }

            if (distAB + distA1B1 < distAA1 + distBB1)
            {
                if (inst->verbose >= 90) 
                    printf("[2optLoop] Swapping edges %d-%d and %d-%d with %d-%d and %d-%d\n", *A, *A1, *B, *B1, *A1, *B1, *B, *A);

                // Reverse the sub-array from A to B
                reverseSubvector(A1, B);

                *cost = *cost - (distAA1 + distBB1) + (distAB + distA1B1);
                counter = 0;

                // update official solution
                if(*cost < inst->zbest){
                    if(bestSolution(result, *cost, inst) != 0 && VNS){
                        if (inst->verbose >= 60) 
                            printf("[2optLoop] Optimization NOT completed, time limit reached.\n\n");
                        free(result);
                        return 1;
                    }
                }

                if(plotFlag){
                    costs[*nCosts].x = *xIndex;
                    costs[*nCosts].y = *cost;

                    *nCosts++;
                    if(!VNS)
                        show_costs(inst, costs, *nCosts, false);

                    point* tmp = (point*)calloc(*nCosts+1, sizeof(point));
                    for(int i=0;i<*nCosts;i++){
                        tmp[i] = costs[i];
                    }

                    free(costs);
                    costs = tmp;
                }
            }
        }
        
        // move A to the end of the array
        int Acopy = *A;
        for(int i=0; i<inst->nnodes-1; i++){
            result[i] = result[i+1];
        }

        result[inst->nnodes-1] = Acopy;

    } while (counter < inst->nnodes);

    free(costs);
    return 0;
}

int twoOpt(instance* inst)
{
    if (inst->verbose >= 80) printf("[2opt] Starting initialization.\n");

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

    clock_t end;
    double time;
    
    if (inst->verbose >= 80) printf("[2opt] Initialization completed, starting optimization.\n");

    twoOptLoop(inst, result, &cost, costs, &nCosts, &xIndex, false, false);

    if (inst->verbose >= 80) printf("[2opt] Optimization completed.\n\n");

    bestSolution(result, cost, inst);
    free(result);
    
    return 0;
}

int tabuSearch(instance* inst)
{
    if (inst->verbose >= 80) printf("[Tabu' Search] Starting initialization.\n");

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
    clock_t end;
    double time;
    
    if (inst->verbose >= 80) printf("[Tabu' Search] Initialization completed, starting optimization.\n");

    do
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
                    if (bestCost == -1 && inst->verbose >= 80) 
                        printf("[Tabu' Search] First feasible solution found, saving it\n");
                    
                    // copy current solution
                    copyArray(result, bestSol, inst->nnodes);

                    // do swap and update costs
                    reverseSubvector(&bestSol[A], &bestSol[B]);
                    bestCost = cost - old + new;

                    // save swapped node
                    swappedNode = result[A];

                    if (inst->verbose >= 90)
                        printf("[Tabu' Search] Swapping nodes %d and %d. New cost is %f\n", result[A], result[B], bestCost);
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

            if (inst->verbose >= 80) 
                printf("[Tabu' Search] The new solution is worse, adding it to tabu' list\n");
        }

        if (inst->verbose >= 80) 
            printf("[Tabu' Search] Updating new 'current' solution. Old cost: %f, New cost: %f\n", cost, bestCost);
        
        copyArray(bestSol, result, inst->nnodes);
        cost = bestCost;

        if(cost < inst->zbest)
        {
            if (inst->verbose >= 80) 
                printf("[Tabu' Search] Updating best official solution.\n");
        
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

        if (inst->verbose >= 80) printf("[Tabu' Search] time: %f, limit:%f\n", time, inst->time_limit);

        //system("pause");

    }while(time < inst->time_limit);

    if (inst->verbose >= 80) printf("[Tabu' Search] Optimization completed.\n\n");

    free(result);
    free(tabuList);
    free(costs);

    return 0;
}

int variableNeighborhoodSearch(instance* inst)
{
    // time checkers
    clock_t end;
    double time;

    //result vector initialization
    if (inst->verbose >= 80) printf("[VNS] Starting initialization.\n");
    
    int* result = (int*) calloc(inst->nnodes, sizeof(int));
    double cost = inst->zbest;
    for (int i = 0; i < inst->nnodes; i++)
        result[i] = inst->best_sol[i];
    
    inst->plotCosts = NULL;
    point* costs = (point*)calloc(1, sizeof(point));
    int nCosts = 0, xIndex = 0;
    bool plotFlag = false;

    if (inst->verbose >= 80) printf("[VNS] Initialization completed, starting optimization.\n");

    // cicle 2opt + kick
    do{
            
        // 2OPT SECTION

        twoOptLoop(inst, result, &cost, costs, &nCosts, &xIndex, true, false);

        if (inst->verbose >= 80) printf("[VNS - 2opt] Optimization completed, cost: %f, kicking the solution.\n", cost);

        // KICKS SECTION
        int nkicks = (rand() % 9) + 2; // range 2-10
        for(;nkicks > 0; nkicks--){
            kick(&cost, result, inst);
        }

        xIndex++;
        if(plotFlag){
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

        if (inst->verbose >= 80) printf("[VNS - 2opt] Kicks done, cost: %f.\n\n", cost);

        // TIME CHECK
        end = clock();
        time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;

        if (inst->verbose >= 90) printf("[VNS] time: %f, limit:%f\n", time, inst->time_limit);

    }while(time < inst->time_limit);
    
    free(result);
    
    return 0;
}
