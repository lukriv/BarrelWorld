#ifndef __GAME_PHYSICS_COMPONENT_H__
#define __GAME_PHYSICS_COMPONENT_H__


#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include "../gcompbase.h"

class btCollisionObject;


class PhysicsComponent : public ComponentBase {
	
	GameEntity *m_pParent;
	btCollisionObject *m_pColObject;
	
public:

	PhysicsComponent(btCollisionObject *pColObject) : ComponentBase(GAME_COMP_PHYSICS)
						, m_pParent(nullptr)
						, m_pColObject(pColObject) {}
	~PhysicsComponent() {}
	
	GameErrorCode Initialize();
	
	inline btCollisionObject * GetCollisionObject() { return m_pColObject; }
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity);
	virtual GameErrorCode Update();

	
};

#endif //__GAME_PHYSICS_COMPONENT_H__