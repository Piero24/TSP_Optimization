#ifndef VNS_H
#define VNS_H

#include "twoOpt.h"
#include "optUtils.h"
#include "../../tsp.h"

/**
 * @brief 2opt moves with kicks when the best solution gets stuck in local minimum
 * 
 * @param inst Pointer to the instance structure.
 * 
 * @return 0 if the algorithm runs successfully, 1 if an error occurs.
 */
int variableNeighborhoodSearch(instance* inst);

#endif /* VNS_H */