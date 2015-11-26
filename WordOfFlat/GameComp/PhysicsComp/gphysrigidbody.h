#ifndef __GAME_PHYSICS_RIGID_BODY_H__
#define __GAME_PHYSICS_RIGID_BODY_H__

#include "../gcompbase.h"
#include <bullet/LinearMath/btScalar.h>
#include <GameSystem/refobjectsmptr.h>

class PhysicsSystem;
class btCollisionShape;
class btRigidBody;
class TransformComponent;
class wxXmlNode;

class PhysicsRigidBody : public PhysicsBase 
{
protected:
    
    btRigidBody* m_pRigidBody;
	
public:

    PhysicsRigidBody(PhysicsSystem* pOwnerMgr);
    ~PhysicsRigidBody();
	
	GameErrorCode Create(btScalar mass, btCollisionShape *pColShape);
	
	GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
    virtual GameErrorCode Update();
};

#endif // __GAME_PHYSICS_RIGID_BODY_H__
