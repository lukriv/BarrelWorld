#ifndef __GAME_PHYSHICS_BASE_H__
#define __GAME_PHYSHICS_BASE_H__

#include "gphyssystem.h"
#include "../transformComp/gtranscomp.h"
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>

class PhysicsBase : public ComponentBase {
protected:
	GamePhysicsSystem* m_pPhysSystem;
	RefObjSmPtr<TransformComponent> m_spTransform;
	wxString m_definitionFile; //!< Name of physics definition file - for load and store component (empty string = shape definition was used)
public:
	PhysicsBase(GamePhysicsSystem* pPhysSystem);
	virtual ~PhysicsBase();
	
	GameErrorCode Initialize(TransformComponent *pTransform);
	
	virtual btRigidBody* GetRigidBody() = 0;

};

#endif // __GAME_PHYSHICS_BASE_H__
