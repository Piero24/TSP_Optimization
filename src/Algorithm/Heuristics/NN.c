#include "Algorithm/Heuristics/NN.h"

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
            verbose_print(inst, 80, "[NNFromEachNode] New best solution founded starting from %d with cost %f\n", firstNode, cost);

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

    verbose_print(inst, 80, "[Nearest Neighbor] Starting initialization\n");

    *cost = 0;
    for(int i=0; i<inst->nnodes; i++)
    {
        result[i] = i;
    }

    result[0] = firstNode;
    result[firstNode] = 0;

    verbose_print(inst, 80, "[Nearest Neighbor] Initialization completed, starting the algorithm\n");

    for(int current=0; current<inst->nnodes-1; current++)
    {
        verbose_print(inst, 95, "[Nearest Neighbor] Current node: %d\n", result[current]);

        double minDist = dist(inst, result[current], result[current+1]) * weights[xpos(result[current], result[current+1], inst)];
        int nearest = current+1;

        for(int next=current+2; next<inst->nnodes; next++)
        {
            if(dist(inst, result[current], result[next]) * weights[xpos(result[current], result[next], inst)] < minDist)
            {
                minDist = dist(inst, result[current], result[next]) * weights[xpos(result[current], result[next], inst)];
                nearest = next;
            }
        }

        int tmp = result[current+1];
        result[current+1] = result[nearest];
        result[nearest] = tmp;

        *cost += dist(inst, result[current], result[nearest]);
        verbose_print(inst, 95, "[Nearest Neighbor] Current cost: %f\n", *cost);
    }

    *cost += dist(inst, result[0], result[inst->nnodes-1]);
    verbose_print(inst, 95, "[Nearest Neighbor] Current cost: %f\n", *cost);

    verbose_print(inst, 80, "[Nearest Neighbor] Algorithm finished\n\n");
    
    return 0;
}