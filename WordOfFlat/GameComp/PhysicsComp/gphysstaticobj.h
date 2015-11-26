#ifndef __GAME_PHYSICS_STATIC_OBJECT_H__
#define __GAME_PHYSICS_STATIC_OBJECT_H__

#include "gcompbase.h" // Base class: ComponentBase
#include <GameSystem/refobjectsmptr.h>


class btCollisionObject;
class btCollisionShape;
class wxXmlNode;
class TransformComponent;
class PhysicsSystem;

class PhysicsStaticObject : public PhysicsBase
{
protected:
	btCollisionObject *m_pColObject;
	
public:
	PhysicsStaticObject(PhysicsSystem* pPhysSystem);
	~PhysicsStaticObject();
	
	GameErrorCode Create(btCollisionShape *pColShape);
	
    virtual GameErrorCode Load(wxXmlNode* XMLNode);
    virtual GameErrorCode Store(wxXmlNode* ParentNode);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	virtual GameErrorCode Update();

};

#endif // __GAME_PHYSICS_STATIC_OBJECT_H__
