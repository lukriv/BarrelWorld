#include "gentity.h"

GameEntity::GameEntity() : m_id(MAXDWORD)
{}

GameEntity::GameEntity(const GameEntity& master)
{
	// copy name
	m_id = master.m_id;
	
	// copy components
	m_componentList=master.m_componentList;
}

GameEntity::~GameEntity()
{}


ComponentBase* GameEntity::GetComponent(GameComponentType compType)
{
	//lock
	wxCriticalSectionLocker lock(m_entityLock);
	
	
	return m_componentList[compType].In();
}

GameErrorCode GameEntity::AddComponent(ComponentBase* pComp)
{
	if(!pComp)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	//lock
	wxCriticalSectionLocker lock(m_entityLock);
	
	m_componentList[pComp->GetComponentType()] = pComp;
	
	pComp->SetParentEntity(this);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntity::ReceiveMessage(TaskMessage& msg)
{
	// lock
	wxCriticalSectionLocker lock(m_entityLock);
	
	TCompMap::Iterator endIter = m_componentList.End();
	for( TCompMap::Iterator iter = m_componentList.Begin(); iter != endIter; ++iter)
	{
		if(!iter->second.IsEmpty())
		{
			iter->second->ReceiveMessage(msg);
		}
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntity::RemoveComponent(GameComponentType compType)
{
	// lock
	wxCriticalSectionLocker lock(m_entityLock);
	
	m_componentList[compType]->SetParentEntity(nullptr);
	m_componentList[compType].Release();
	
	return FWG_NO_ERROR;
}

void GameEntity::Clear()
{
	m_componentList.Clear();
}
