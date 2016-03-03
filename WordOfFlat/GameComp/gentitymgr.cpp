#include "gentitymgr.h"
#include "gcompmgr.h"

GameEntityManager::GameEntityManager(GameLogger* pLogger, GameCompManager& compMgr) : m_spLogger(pLogger)
														, m_nextFreeId(1)
														, m_tranformMgr(pLogger, this)
														, m_logicControllerMgr(&compMgr.GetLogicSystem(), this)
														, m_physicsMgr(&compMgr.GetPhysicsSystem(), this)
														, m_physicsControllerMgr(&compMgr.GetPhysicsSystem(), this)
														, m_renderPosMgr(&compMgr.GetRenderSystem(), this)
														, m_renderObjMgr(&compMgr.GetRenderSystem(), this)
{
}

GameEntityManager::~GameEntityManager()
{
	DestroyAllEntities();
}

wxDword GameEntityManager::GetNewId()
{
	wxDword newId = 0;
	wxCriticalSectionLocker lock(m_mgrLock);
	if(m_freeIds.isEmpty())
	{
		newId = m_nextFreeId++;
	} else {
		newId = m_freeIds.front();
		m_freeIds.pop();
	}
	
	return newId;
	
}

void GameEntityManager::DestroyEntity(wxDword entityId)
{
	wxCriticalSectionLocker lock(m_mgrLock);
	
	m_tranformMgr.DestroyComponent(entityId);
	m_logicControllerMgr.DestroyComponent(entityId);
	m_physicsMgr.DestroyComponent(entityId);
	m_renderPosMgr.DestroyComponent(entityId);
	m_renderObjMgr.DestroyComponent(entityId);
	
	m_freeIds.push(entityId);
	
}

void GameEntityManager::DestroyAllEntities()
{
	wxCriticalSectionLocker lock(m_mgrLock);
	
	m_tranformMgr.DestroyAllComponents();
	m_logicControllerMgr.DestroyAllComponents();
	m_physicsMgr.DestroyAllComponents();
	m_physicsControllerMgr.DestroyAllComponents();
	m_renderPosMgr.DestroyAllComponents();
	m_renderObjMgr.DestroyAllComponents();
	
	m_nextFreeId = 1;
	while(!m_freeIds.isEmpty())
	{
		m_freeIds.pop();
	}
	
}


GameErrorCode GameEntityManager::SendTaskMessage(wxDword receiverID, TaskMessage& msg)
{
	ComponentBase *pComp = nullptr;
	pComp = m_tranformMgr.GetComponent(receiverID);
	if(pComp) FWG_RETURN_FAIL(pComp->ReceiveMessage(msg));
	
	pComp = m_logicControllerMgr.GetComponent(receiverID);
	if(pComp) FWG_RETURN_FAIL(pComp->ReceiveMessage(msg));
	
	pComp = m_physicsMgr.GetComponent(receiverID);
	if(pComp) FWG_RETURN_FAIL(pComp->ReceiveMessage(msg));
	
	pComp = m_renderPosMgr.GetComponent(receiverID); 
	if(pComp) FWG_RETURN_FAIL(pComp->ReceiveMessage(msg));
	
	pComp = m_renderObjMgr.GetComponent(receiverID); 
	if(pComp) FWG_RETURN_FAIL(pComp->ReceiveMessage(msg));
	
	return FWG_NO_ERROR;
	
}
