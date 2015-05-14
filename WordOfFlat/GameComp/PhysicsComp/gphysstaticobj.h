#ifndef __GAME_PHYSICS_STATIC_OBJECT_H__
#define __GAME_PHYSICS_STATIC_OBJECT_H__

#include "gcompbase.h" // Base class: ComponentBase
#include <GameSystem/refobjectsmptr.h>


class btCollisionObject;
class btCollisionShape;
class wxXmlNode;
class TransformComponent;
class PhysicsCompManager;

class PhysicsStaticObject : public ComponentBase
{
protected:
	PhysicsCompManager *m_pOwnerMgr;
	btCollisionObject *m_pColObject;
	
	RefObjSmPtr<TransformComponent> m_spTransform;
public:
	PhysicsStaticObject(PhysicsCompManager* pOwnerMgr);
	~PhysicsStaticObject();
	
	GameErrorCode Initialize(TransformComponent *pTransform);
	
	GameErrorCode Create(btCollisionShape *pColShape);
	
    GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	virtual GameErrorCode Update();

};

#endif // __GAME_PHYSICS_STATIC_OBJECT_H__
