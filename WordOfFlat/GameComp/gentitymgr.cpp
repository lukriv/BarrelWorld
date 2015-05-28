#include "gentitymgr.h"

GameEntityManager::GameEntityManager(GameLogger* pLogger) : m_spLogger(pLogger)
{
}

GameEntityManager::~GameEntityManager()
{
	DestroyAllEntities();
}

GameEntity* GameEntityManager::GetEntity(wxDword entityId)
{
	return m_entityMap.GetItem(entityId);
}

GameErrorCode GameEntityManager::CreateEntity(GameEntity*& pNewEntity)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxDword entId = 0;
	if(FWG_FAILED(result = m_entityMap.AllocNewItem(entId)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Cannot alloc new entity item"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	pNewEntity = m_entityMap.GetItem(entId);
	pNewEntity->SetId(entId);
	
	return FWG_NO_ERROR;
}

void GameEntityManager::DestroyEntity(wxDword entityId)
{
	//m_entityMap.Remove(entityName);
	GameEntity *pEntity = m_entityMap.GetItem(entityId);
	pEntity->Clear();
	m_entityMap.FreeItem(entityId);
}

void GameEntityManager::DestroyEntity(GameEntity* pEntity)
{
	pEntity->Clear();
	m_entityMap.FreeItem(pEntity->GetId());
}


void GameEntityManager::DestroyAllEntities()
{
	//m_entityMap.Clear();
	m_entityMap.FreeAll();
}



