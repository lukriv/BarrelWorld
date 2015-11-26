#ifndef __GAME_PHYSHICS_BASE_H__
#define __GAME_PHYSHICS_BASE_H__

#include "gphyssystem.h"


class PhysicsBase : public ComponentBase {
protected:
	PhysicsSystem* m_pPhysSystem;
	RefObjSmPtr<TransformComponent> m_spTransform;
public:
	PhysicsBase(PhysicsSystem* pPhysSystem);
	virtual ~PhysicsBase();
	
	GameErrorCode Initialize(TransformComponent *pTransform);

};

#endif // __GAME_PHYSHICS_BASE_H__
