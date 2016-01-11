#ifndef __GAME_PHYSICS_RIGID_BODY_H__
#define __GAME_PHYSICS_RIGID_BODY_H__


#include <bullet/LinearMath/btScalar.h>
#include <GameSystem/refobjectsmptr.h>
#include "../gcompbase.h"
#include "gphysbase.h"

class GamePhysicsSystem;
class btCollisionShape;
class btRigidBody;
class TransformComponent;
class wxXmlNode;

class PhysicsRigidBody : public PhysicsBase 
{
protected:
    
    btRigidBody* m_pRigidBody;
	
public:

    PhysicsRigidBody(GamePhysicsSystem* pOwnerMgr);
    ~PhysicsRigidBody();
	
	GameErrorCode Create(btScalar mass, btCollisionShape *pColShape);
	
	GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
    virtual GameErrorCode Update();
};

#endif // __GAME_PHYSICS_RIGID_BODY_H__
