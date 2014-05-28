#include "gentitymgr.h"

GameEntityManager::GameEntityManager(GameLogger* pLogger) : m_spLogger(pLogger)
{
}

GameEntityManager::~GameEntityManager()
{
	DestroyAllEntities();
}

GameEntity* GameEntityManager::GetEntity(const wxString& entityName)
{
	return m_entityMap.FindValue(entityName);
}

GameErrorCode GameEntityManager::CreateEntity(const wxString& entityName, GameEntity*& pNewEntity)
{
	TEntityMap::Iterator iter;
	if(FWG_FAILED(m_entityMap.Insert(entityName, GameEntity(), iter)))
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	pNewEntity = &(iter->second);
	pNewEntity->SetName(entityName);
	
	return FWG_NO_ERROR;
}

void GameEntityManager::DestroyEntity(const wxString& entityName)
{
	m_entityMap.Remove(entityName);
}

void GameEntityManager::DestroyEntity(GameEntity* entity)
{
	TEntityMap::Iterator iter;
	
	iter = m_entityMap.Find(entity->GetName());
	if(iter != m_entityMap.End())
	{
		m_entityMap.Remove(iter);
	}
}


void GameEntityManager::DestroyAllEntities()
{
	m_entityMap.Clear();
}



