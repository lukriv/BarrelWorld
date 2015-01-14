#ifndef __GAME_PHYSICS_COMPONENT_H__
#define __GAME_PHYSICS_COMPONENT_H__

#include <bullet/btBulletDynamicsCommon.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include "../gcompbase.h"

class btCollisionObject;
class PhysicsCompManager;

class PhysicsComponent : public ComponentBase {
	PhysicsCompManager *m_pOwnerMgr;
	GameEntity *m_pParent;
	btRigidBody *m_pColObject;
	
public:

	PhysicsComponent(PhysicsCompManager *pOwnerMgr) : ComponentBase(GAME_COMP_PHYSICS)
						, m_pOwnerMgr(pOwnerMgr)
						, m_pParent(nullptr)
						, m_pColObject(nullptr) {}
	~PhysicsComponent();
	
	GameErrorCode Initialize(GameEntity *pParentEntity, btRigidBody *pColObj);
	
	inline btRigidBody * GetRigitBody() { return m_pColObject; }
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity);
	virtual GameErrorCode Update();

	
};

#endif //__GAME_PHYSICS_COMPONENT_H__