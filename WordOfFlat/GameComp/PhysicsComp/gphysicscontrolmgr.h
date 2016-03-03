#ifndef __GAME_PHYSICS_CONTROLLER_H__
#define __GAME_PHYSICS_CONTROLLER_H__

#include "../gmanagerbase.h"
#include "gphyscontrol.h"


class TransformComponent;
class InputHandler;

class PhysicsControllerManager : public GameManagerBase<PhysicsController>
{
private:
	GamePhysicsSystem *m_pPhysicsSystem;
public:
	PhysicsControllerManager(GamePhysicsSystem* pPhysicsSystem, GameEntityManager *pEntityMgr);
	~PhysicsControllerManager();

	
	GameErrorCode AddPhysicsController(wxDword compId, PhysicsController *pController )
	{
		
		FWG_RETURN_FAIL(InsertToMap(compId, pController));
		
		FWG_RETURN_FAIL(pController->_initialize(m_pPhysicsSystem));
		
		return FWG_NO_ERROR;
	}

};



#endif // __GAME_PHYSICS_CONTROLLER_H__
