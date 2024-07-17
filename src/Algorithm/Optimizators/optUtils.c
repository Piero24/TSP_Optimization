#include "Algorithm/Optimizators/optUtils.h"

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
void kick(double* cost, int* result, instance* inst)
{
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
