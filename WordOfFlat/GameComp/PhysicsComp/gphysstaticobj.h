#ifndef __GAME_PHYSICS_STATIC_OBJECT_H__
#define __GAME_PHYSICS_STATIC_OBJECT_H__

#include "gcompbase.h" // Base class: ComponentBase

class btCollisionObject;

class PhysicsStaticObject : public ComponentBase
{
protected:
	PhysicsCompManager m_pOwnerMgr;
	btCollisionObject *m_pColObject;
public:
	PhysicsStaticObject(PhysicsCompManager* pOwnerMgr);
	~PhysicsStaticObject();
	
	
	GameErrorCode Create();

};

#endif // __GAME_PHYSICS_STATIC_OBJECT_H__
