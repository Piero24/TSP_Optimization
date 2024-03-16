#include <stdio.h>
#include <stdlib.h>

#include "../include/Algorithm/NN.h"

//firstNode index of starting node (from 0 to nnodes-1)
int nearestNeighbor(int* result, double* cost, instance* inst, int firstNode)
{
    if(firstNode < 0 || firstNode > inst->nnodes)
    {
        printf("fristNode must be between 0 and %d", inst->nnodes);
        return 1;
    }

    // if(VERBOSE > 100) printf("Initialize result.\n");

    *cost = 0;
    for(int i=0; i<inst->nnodes; i++)
    {
        result[i] = i;
    }

    result[0] = firstNode;
    result[firstNode] = 0;

    // if(VERBOSE > 500) printf("Result vector initialized.\n");

    for(int current=0; current<inst->nnodes-1; current++)
    {
        // if(VERBOSE > 1000) printf("[NN] Current node: %d\n", current);

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

int optimization(int* result, double* cost, instance* inst){
    // check every quartet of nodes A, B, C and D such that D is the predecessor of A,
    // C is the successor of B and there is a path starting from A to B that does not 
    // go through C and D

    // visualization:   | - | - | D | A | - | - | - | B | C | - | - | - |
    //         path between A and B-->  ^^^^^^^^^^^^^

    // we try to see if swapping A-D and B-C with A-C and B-D would reduce the cost
    // that is if d(A,C) + d(B,D) < d(A,D) + d(B,C)
    // if so, we invert all the sub-array from A to B

    // from:            | - | - | D | A | x | y | z | B | C | - | - | - |
    // to:              | - | - | D | B | z | y | x | A | C | - | - | - |

    //pseudocode:
    /*

    for every node A:
        for every node B > A:  
        
        // Warning, B may be before A if it goes around the array, 
        // es | - | - | B | C | - | - | - | D | A | - | - | - |
        //    ^^^^^^^^^                           ^^^^^^^^^^^^^

            let D be A-1
            let C be B+1

            if d(A,C) + d(B,D) < d(A,D) + d(B,C):
                result = result[:D] + reverse(result[A:B]) + result[C:]
    
    repeat until no swap after a full cicle of A

    */
   return -1;
}

int tabuSearch(int* result, double* cost, instance* inst){
    /*
    
    todo:
    1. search in a neighborhood of solutions
    
    2. if in the neighborhood there is a solution better than the current one:
        2.1 move to the best solution of the neighborhood
        2.2 if it's better that the current best one, update it
    3. if there are no better solutions in the neighborhood:
        3.1 put current in tabuList
        3.2 move to best solution in the neighborhood that is not in the tabuList
    4. goto 1
    
    initially the best one is the starting array

    
    generation of neighborhood:
        depends on neighborhood dimension, that is the number of different edges (can we 
        choose a different meaning of dimension? "number of different nodes" would be much easier)

        example with dim 3 (minimum?):
            nodes:    | A | B | C | D | E | F | G | H | I | J | K | A |
            edges:      |A-B|B-C|C-D|D-E|E-F|F-G|G-H|H-I|I-J|J-K|K-A|

            swap 2 consegutive nodes
                                       vvv vvv
            nodes:    | A | B | C | D | F | E | G | H | I | J | K | A |
            edges:      |A-B|B-C|C-D|D-F|F-E|E-G|G-H|H-I|I-J|J-K|K-A|
                                     ^^^ ^^^ ^^^
    */

    return -1;
}