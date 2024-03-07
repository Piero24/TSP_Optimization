#include "../include/Algorithm/NN.h"
#include <stdio.h>
#include <stdlib.h>

//firstNode index of starting node (from 0 to nnodes-1)
int nearestNeighbor(int* result, double* cost, instance* inst, int firstNode)
{
    if(firstNode < 0 || firstNode > inst->nnodes)
    {
        printf("fristNode must be between 0 and %d", inst->nnodes);
        return 1;
    }

    if(VERBOSE > 100) printf("Initialize result.\n");

    *cost = 0;
    for(int i=0; i<inst->nnodes; i++)
    {
        result[i] = i;
    }

    result[0] = firstNode;
    result[firstNode] = 0;

    if(VERBOSE > 500) printf("Result vector initialized.\n");

    for(int current=0; current<inst->nnodes-1; current++)
    {
        if(VERBOSE > 1000) printf("[NN] Current node: %d\n", current);

        double minDist = inst->distances[result[current]][result[current+1]];
        int nearest = current+1;

        for(int next=current+2; next<inst->nnodes; next++)
        {
            if(inst->distances[result[current]][result[next]] < minDist)
            {
                minDist = inst->distances[result[current]][result[next]];
                nearest = next;
            }
        }

        int tmp = result[current+1];
        result[current+1] = result[nearest];
        result[nearest] = tmp;

        *cost += inst->distances[result[current]][result[nearest]];
    }

    *cost += inst->distances[result[0]][result[inst->nnodes-1]];
    return 0;
}