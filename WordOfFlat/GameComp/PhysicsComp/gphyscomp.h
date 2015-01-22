#ifndef __GAME_PHYSICS_COMPONENT_H__
#define __GAME_PHYSICS_COMPONENT_H__

#include <bullet/btBulletDynamicsCommon.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include "../gcompbase.h"

class btCollisionObject;
class PhysicsCompManager;
//class TransformComponent;

class PhysicsComponent : public ComponentBase {
protected:
	enum PhysicsType {
		PHYS_COMP_TYPE_UNKNOWN		= 0,
		PHYS_COMP_TYPE_RIGID		= 1,
		PHYS_COMP_TYPE_COLLISION	= 2
	};
protected:
	PhysicsType m_type;
	PhysicsCompManager *m_pOwnerMgr;
	GameEntity *m_pParent;
	btCollisionObject *m_pColObject;
	//RefObjSmPtr<TransformComponent> m_spTransform;

protected:
	GameErrorCode InitializeInternal (GameEntity *pParentEntity, btCollisionObject *pColObj);
	
public:

	PhysicsComponent(PhysicsCompManager *pOwnerMgr) : ComponentBase(GAME_COMP_PHYSICS)
						, m_type(PHYS_COMP_TYPE_UNKNOWN)
						, m_pOwnerMgr(pOwnerMgr)
						, m_pParent(nullptr)
						, m_pColObject(nullptr) {}
	~PhysicsComponent();
	
	GameErrorCode Initialize(GameEntity *pParentEntity, btCollisionObject *pColObj);
	GameErrorCode Initialize(GameEntity *pParentEntity, btRigidBody *pColObj);
	
	inline bool IsCollision() { return (m_type == PHYS_COMP_TYPE_COLLISION); }
	inline btCollisionObject * GetCollsionObject() { return m_pColObject; }
	
	inline bool IsRigidBody() { return (m_type == PHYS_COMP_TYPE_RIGID); }
	inline btRigidBody * GetRigitBody() { return (m_type == PHYS_COMP_TYPE_RIGID)? reinterpret_cast<btRigidBody*>(m_pColObject) : nullptr; }
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity);
	virtual GameErrorCode Update();

	
};

#endif //__GAME_PHYSICS_COMPONENT_H__