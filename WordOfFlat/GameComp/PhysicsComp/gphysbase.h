#ifndef __GAME_PHYSHICS_BASE_H__
#define __GAME_PHYSHICS_BASE_H__

#include "gphyssystem.h"
#include "../transformComp/gtranscomp.h"


class PhysicsBase : public ComponentBase {
protected:
	GamePhysicsSystem* m_pPhysSystem;
	RefObjSmPtr<TransformComponent> m_spTransform;
public:
	PhysicsBase(GamePhysicsSystem* pPhysSystem);
	virtual ~PhysicsBase();
	
	GameErrorCode Initialize(TransformComponent *pTransform);

};

#endif // __GAME_PHYSHICS_BASE_H__
