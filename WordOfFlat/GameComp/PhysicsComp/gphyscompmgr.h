#ifndef __GAME_PHYSICS_SYSTEM_MANAGER_H__
#define __GAME_PHYSICS_SYSTEM_MANAGER_H__


#include <GameComp/gmanagerbase.h>
#include "gphyssystem.h"
#include "gphysbase.h"



class PhysicsCompManager : public GameManagerBase<PhysicsBase>
{
private:
	GamePhysicsSystem* m_pPhysSystem;
public:
	PhysicsCompManager(GamePhysicsSystem* pPhysicsSystem, GameEntityManager *pEntityMgr);
	~PhysicsCompManager();
	
	template <class T>
	GameErrorCode CreateComponent(wxDword compId, TransformComponent* trans, T *&pNewComponent )
	{
		RefObjSmPtr<T> spPhysComp;
		FWG_RETURN_FAIL(GameNewChecked(spPhysComp.OutRef(), m_pPhysSystem));
		
		FWG_RETURN_FAIL(spPhysComp->Initialize(trans));
		
		FWG_RETURN_FAIL(InsertToMap(compId, spPhysComp));
		
		// todo: insert component to logic system
		
		pNewComponent = spPhysComp.Detach();
		
		return FWG_NO_ERROR;
	}

};

#endif // __GAME_PHYSICS_SYSTEM_MANAGER_H__