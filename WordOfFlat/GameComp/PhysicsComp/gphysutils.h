#ifndef __GAME_PHYSICS_UTILITIES__H__
#define __GAME_PHYSICS_UTILITIES__H__

#include <GameSystem/gerror.h>
#include "gphyssystem.h"
#include "gphysbase.h"


class GamePhysicsUtils
{
public:
	/**
	 * @brief Compute collision component with ground
	 * @param physSystem physics system
	 * @param physComponent kinematics avatar
	 * 
	 * @return How far is object above ground ( or another object )
	 */
	static btScalar ComputeGroundDistance(GamePhysicsSystem &physSystem, PhysicsBase &physComponent);

};

#endif // __GAME_PHYSICS_UTILITIES__H__
