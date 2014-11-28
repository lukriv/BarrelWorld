#ifndef __GAME_PHYSICS_COMPONENT_H__
#define __GAME_PHYSICS_COMPONENT_H__


#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include "../gcompbase.h"


class PhysicsComponent : public ComponentBase {
	
	GameEntity *m_pParent;
public:

	PhysicsComponent() : ComponentBase(GAME_COMP_PHYSICS), m_pParent(nullptr) {}
	~PhysicsComponent() {}
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity);
	virtual GameErrorCode Update();

	
};

#endif //__GAME_PHYSICS_COMPONENT_H__