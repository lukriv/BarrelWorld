#ifndef __CHARACTER_PHYSICS_CONTROL_H__
#define __CHARACTER_PHYSICS_CONTROL_H__

// Base class: LogicComponentBase
#include <bullet/LinearMath/btScalar.h>
#include <GameComp/transformComp/gtranscomp.h>
#include <GameComp/PhysicsComp/gphyscontrol.h>
#include "ginputfreecam.h"
#include "ginputchar.h"


class CharacterController : public PhysicsController
{
	RefObjSmPtr<CharacterInput> m_spCharInput;
	btScalar m_diffForwardBackward;
	btScalar m_diffLeftRight;
	
public:
	CharacterController(CharacterInput *pInput);
	~CharacterController();

	virtual	void debugDraw(btIDebugDraw* debugDrawer);
	virtual void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

};

#endif // __CHARACTER_PHYSICS_CONTROL_H__
