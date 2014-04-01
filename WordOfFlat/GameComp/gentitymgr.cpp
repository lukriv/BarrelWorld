#include "gentitymgr.h"

GameEntity* GameEntityManager::CreateEntity(const wxString& entityName)
{
	TEntityMap::Iterator iter;
	if(FWG_FAILED(m_entityMap.Insert(entityName, GameEntity(), iter)))
	{
		return NULL;
	}
	return &(iter->second);
}

GameEntity* GameEntityManager::GetEntity(const wxString& entityName)
{
	return m_entityMap.FindValue(entityName);
}

void GameEntityManager::RemoveEntity(const wxString& entityName)
{
	m_entityMap.Remove(entityName);
}
