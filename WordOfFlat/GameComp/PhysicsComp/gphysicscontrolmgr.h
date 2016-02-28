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

	
	template <class T>
	GameErrorCode CreateComponent(wxDword compId, TransformComponent *pTransform, InputHandler *pInput, T *&pNewComponent )
	{
		RefObjSmPtr<T> spPhysicsController;
		FWG_RETURN_FAIL(GameNewChecked(spPhysicsController.OutRef(), m_pPhysicsSystem));
		
		FWG_RETURN_FAIL(spPhysicsController->Initialize(pTransform));
		
		FWG_RETURN_FAIL(InsertToMap(compId, spPhysicsController));
		
		// todo: insert component to logic system
		pNewComponent = spPhysicsController.Detach();
		
		return FWG_NO_ERROR;
	}

};



#endif // __GAME_PHYSICS_CONTROLLER_H__
