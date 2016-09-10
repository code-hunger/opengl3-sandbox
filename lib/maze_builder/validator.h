#ifndef VALIDATOR_H_FOTCG6JB
#define VALIDATOR_H_FOTCG6JB

#include "def.h"
#define FORCE_VALIDATE false

void validate_cross_road(CrossRoad* crossRoad);
void validate_walls(Walls& walls, bool force = FORCE_VALIDATE);

#endif /* end of include guard: VALIDATOR_H_FOTCG6JB */
