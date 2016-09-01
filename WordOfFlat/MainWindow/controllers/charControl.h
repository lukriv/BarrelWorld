#ifndef __CHARACTER_PHYSICS_CONTROL_H__
#define __CHARACTER_PHYSICS_CONTROL_H__

// Base class: LogicComponentBase
#include <bullet/LinearMath/btScalar.h>
#include <GameComp/transformComp/gtranscomp.h>
#include <GameComp/PhysicsComp/gphyscontrol.h>
#include <GameComp/PropertiesComp/gpropertycomp.h>
#include "ginputfreecam.h"
#include "ginputchar.h"

class GameCompManager;


class CharacterController : public PhysicsController
{
	GameCompManager *m_pCompMgr;
	RefObjSmPtr<CharacterInput> m_spCharInput;
	RefObjSmPtr<PropertyComponent> m_spPropComp;
	btScalar m_angle;
	
public:
	CharacterController(GameCompManager *pCompMgr, CharacterInput *pInput, PropertyComponent *pPropComp);
	~CharacterController();

	virtual	void debugDraw(btIDebugDraw* debugDrawer);
	virtual void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

private:
	btScalar GetActualAngle();
	void ClickAction();
};

#endif // __CHARACTER_PHYSICS_CONTROL_H__
