#ifndef __GAME_PHYSICS_COMPONENT_MANAGER_H__
#define __GAME_PHYSICS_COMPONENT_MANAGER_H__


#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameComp/gcompbase.h>


class PhysicsCompManager {
	
// IGameComponent interface
public:
	
	GameErrorCode CreatePhysicsComponent();
	
	GameErrorCode GetPhysicsComponent();
	
	GameErrorCode StepPhysics();
	
	GameErrorCode ProcessTask();
	
	
	
	GameErrorCode Update(PhysicsComponent* updatedComp);
	

	
};

#endif //__GAME_PHYSICS_COMPONENT_MANAGER_H__