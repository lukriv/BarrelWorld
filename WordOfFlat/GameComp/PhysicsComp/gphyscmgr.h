#ifndef __GAME_PHYSICS_COMPONENT_MANAGER_H__
#define __GAME_PHYSICS_COMPONENT_MANAGER_H__


#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"


class GamePhysicsCompMgr {
	
// IGameComponent interface
public:
	
	GameErrorCode CreatePhysicsComponent();
	
	GameErrorCode GetPhysicsComponent();
	
	GameErrorCode StepPhysics();
	

	
};

#endif //__GAME_PHYSICS_COMPONENT_MANAGER_H__