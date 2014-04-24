#include "gentitymgr.h"

GameEntity* GameEntityManager::GetEntity(const wxString& entityName)
{
	return m_entityMap.FindValue(entityName);
}

void GameEntityManager::RemoveEntity(const wxString& entityName)
{
	m_entityMap.Remove(entityName);
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


void GameEntityManager::DestroyEntity(GameEntity* entity)
{
	TEntityMap::Iterator iter;
	
	iter = m_entityMap.Find(entity->GetName());
	if(iter != m_entityMap.End())
	{
		m_entityMap.Remove(iter);
	}
}
