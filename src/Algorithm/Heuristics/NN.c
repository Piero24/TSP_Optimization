#include "Algorithm/Heuristics/NN.h"

double wDist(instance* inst, double* weights, int a, int b){
    assert(weights[xpos(a, b, inst)] == 1);
    return dist(inst, a, b) * weights[xpos(a, b, inst)];
}

int NNFromEachNode(instance* inst){
    int ncols = inst->nnodes * (inst->nnodes) / 2; //n*(n-1)/2
    double* weights = (double*) malloc(ncols*sizeof(double));
    int* result = (int*) calloc(inst->nnodes, sizeof(int));
    double cost = 0;

    for(int i=0;i<ncols;i++){
        weights[i] = 1;
    }

    int error = WeightedNNFromEachNode(inst, weights, result, &cost, true);
    free(weights);
    return error;
}

int WeightedNNFromEachNode(instance* inst, double* weights, int* result, double* cost, bool updateInst)
{
    int bestFirst = -1;
    double bestCost = -1;

    inst->plotCosts = NULL;
    point* costs = (point*)calloc(1, sizeof(point));
    int nCosts = 0;
    bool plotFlag = false;

    for (int firstNode = 0; firstNode < inst->nnodes; firstNode++){
        weightedNearestNeighbor(inst, firstNode, result, cost, weights);

        if(bestCost == -1 || bestCost > *cost){
            verbose_print(inst, 80, "[NNFromEachNode] New best solution founded starting from %d with cost %f\n", firstNode, *cost);

            bestCost = *cost;
            bestFirst = firstNode;

            inst->start = firstNode;

            if(updateInst){
                if(bestSolution(result, *cost, inst) != 0){
                    verbose_print(inst, 60, "[NNFromEachNode] Algorithm NOT completed, time limit reached.\n\n");
                    free(result);
                    return 1;
                }
            }
        }

        if(plotFlag){
            costs[nCosts].x = firstNode;
            costs[nCosts].y = *cost;

            nCosts++;
            show_costs(inst, costs, nCosts, true);

            point* tmp = (point*)calloc(nCosts+1, sizeof(point));
            for(int i=0;i<nCosts;i++){
                tmp[i] = costs[i];
            }

            free(costs);
            costs = tmp;
        }

        // time check
        double time = ((double) (clock() - inst->tstart)) / CLOCKS_PER_SEC;
        if(time >= inst->time_limit)
            break;
    }

    verbose_print(inst, 70, "[NNFromEachNode] Best solution founded starting from %d with cost %f\n\n", bestFirst, bestCost);
    
    return 0;
}

int nearestNeighbor(instance* inst, int firstNode, int* result, double* cost)
{
    int ncols = inst->nnodes * (inst->nnodes) / 2; //n*(n-1)/2
    double* weights = (double*) malloc(ncols*sizeof(double));

    for(int i=0;i<ncols;i++){
        weights[i] = 1;
    }

    int error = weightedNearestNeighbor(inst, firstNode, result, cost, weights);
    free(weights);
    return error;
}

//firstNode index of starting node (from 0 to nnodes-1)
int weightedNearestNeighbor(instance* inst, int firstNode, int* result, double* cost, double* weights)
{   
    if(firstNode < 0 || firstNode > inst->nnodes)
    {
        printf("fristNode must be between 0 and %d", inst->nnodes);
        return 1;
    }

    verbose_print(inst, 85, "[Nearest Neighbor] Starting initialization\n");

    *cost = 0;
    for(int i=0; i<inst->nnodes; i++)
    {
        result[i] = i;
    }

    result[0] = firstNode;
    result[firstNode] = 0;

    verbose_print(inst, 85, "[Nearest Neighbor] Initialization completed, starting the algorithm\n");

    for(int current=0; current<inst->nnodes-1; current++)
    {
        verbose_print(inst, 95, "[Nearest Neighbor] Current node: %d\n", result[current]);

        double minDist = wDist(inst, weights, result[current], result[current+1]);
        int nearest = current+1;

        for(int next=current+2; next<inst->nnodes; next++)
        {
            if(wDist(inst, weights, result[current], result[next]) < minDist)
            {
                minDist = wDist(inst, weights, result[current], result[next]);
                nearest = next;
            }
        }

        int tmp = result[current+1];
        result[current+1] = result[nearest];
        result[nearest] = tmp;

        *cost += minDist;

        if(inst->debug){
            double costChecker = 0.0;
            for(int i=0;i<current+1; i++){
                costChecker += wDist(inst, weights, result[i], result[i+1]);
            }

            assert(*cost == costChecker);
        }

        verbose_print(inst, 95, "[Nearest Neighbor] Current cost: %f\n", *cost);

        // check time limit
        clock_t end = clock();
        double time = ((double) (end - inst->tstart)) / CLOCKS_PER_SEC;
        if (time >= inst->time_limit) {
            verbose_print(inst, 90, "[Nearest Neighbor] Exiting by time limit\n");
            break;
        }
    }

    *cost += dist(inst, result[0], result[inst->nnodes-1]);
    verbose_print(inst, 95, "[Nearest Neighbor] Current cost: %f\n", *cost);

    if(inst->debug){
        double costChecker = 0.0;
        for(int i=0;i<inst->nnodes-1; i++){
            costChecker += wDist(inst, weights, result[i], result[i+1]);
        }
        costChecker += wDist(inst, weights, result[0], result[inst->nnodes-1]);

        if(fabs(*cost - costChecker) > EPS)
			printf("cost: %f\ncostChecker: %f\n", *cost, costChecker);
		assert(fabs(*cost - costChecker) < EPS);
    }

    verbose_print(inst, 85, "[Nearest Neighbor] Algorithm finished\n\n");
    
    return 0;
}