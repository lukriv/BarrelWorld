#include "gentitymgr.h"

GameEntity* GameEntityManager::CreateEntity(const wxString& entityName)
{
	TEntityMap::Iterator iter;
	FWG_RETURN_FAIL(m_entityMap.Insert(entityName, GameEntity(), iter));
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
