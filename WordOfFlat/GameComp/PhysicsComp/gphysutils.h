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
	static btScalar ComputeGroundDistance(GamePhysicsSystem &physSystem, PhysicsBase &physComponent, wxWord groupFilter, wxWord maskFilter);
	
	
	/**
	 * @brief Compute hit point with given ray
	 * @param physSystem
	 * @param origin Begining of ray
	 * @param point Ray pointing point
	 * @param hitPoint Return hit point
	 * 
	 * @return Returns zero if no object is affected ( ground hit returns also 0 ) or hitted object
	 */
	static wxDword ComputeTargetPoint(GamePhysicsSystem &physSystem, btVector3& origin, btVector3& point, btVector3& hitPoint);

};

#endif // __GAME_PHYSICS_UTILITIES__H__
