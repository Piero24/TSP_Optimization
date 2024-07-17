#ifndef SOFTFIXING_H
#define SOFTFIXING_H

#include "tsp.h"
#include "algoSelector.h"
#include "Algorithm/Exacts/cplexAlg.h"

#include <cplex.h>

/**
 * @brief
 * 
 * @param inst Pointer to the instance containing problem data.
 * 
 * @return Returns 0 upon successful completion, or an appropriate nonzero 
 *  error code if an error occurs.
 */
int localBranching(instance* inst);

#endif /* SOFTFIXING_H */