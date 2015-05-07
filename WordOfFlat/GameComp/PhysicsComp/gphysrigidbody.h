#ifndef __GAME_PHYSICS_RIGID_BODY_H__
#define __GAME_PHYSICS_RIGID_BODY_H__

#include "../gcompbase.h"
#include <bullet/LinearMath/btScalar.h>
#include <GameSystem/refobjectsmptr.h>

class PhysicsCompManager;
class btCollisionShape;
class btRigidBody;
class TransformComponent;

class PhysicsRigidBody : public ComponentBase
{
protected:
    PhysicsCompManager* m_pOwnerMgr;
    btRigidBody* m_pRigidBody;
	
	RefObjSmPtr<TransformComponent> m_spTransform;

public:

    PhysicsRigidBody(PhysicsCompManager* pOwnerMgr);
    ~PhysicsRigidBody();
	
	GameErrorCode Initialize(TransformComponent *pTransform);
	    

	GameErrorCode Create(btScalar mass, btCollisionShape *pColShape);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
    virtual GameErrorCode Update();
};

#endif // __GAME_PHYSICS_RIGID_BODY_H__
