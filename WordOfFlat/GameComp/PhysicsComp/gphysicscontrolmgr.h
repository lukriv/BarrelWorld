#ifndef __GAME_PHYSICS_CONTROLLER_H__
#define __GAME_PHYSICS_CONTROLLER_H__

class PhysicsControllerManager : public GameManagerBase<PhysicsController>
{
private:
	GamePhysicsSystem *m_pPhysicsSystem;
public:
	PhysicsControllerManager(m_pPhysicsSystem* pPhysicsSystem, GameEntityManager *pEntityMgr);
	~PhysicsControllerManager();

	
	template <class T>
	GameErrorCode CreateComponent(wxDword compId, TransformComponent *pTransform, InputComponent *pInput, T *&pNewComponent )
	{
		RefObjSmPtr<T> spLogicComp;
		FWG_RETURN_FAIL(GameNewChecked(spLogicComp.OutRef(), m_pLogicSystem));
		
		FWG_RETURN_FAIL(spLogicComp->Initialize(pTransform, pInput));
		
		FWG_RETURN_FAIL(InsertToMap(compId, spLogicComp));
		
		// todo: insert component to logic system
		pNewComponent = spLogicComp.Detach();
		
		return FWG_NO_ERROR;
	}

};



#endif // __GAME_PHYSICS_CONTROLLER_H__
