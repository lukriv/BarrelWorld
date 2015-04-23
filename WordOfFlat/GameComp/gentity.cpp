#include "gentity.h"

GameEntity::GameEntity() : m_id(MAXDWORD)
{}

GameEntity::GameEntity(const GameEntity& master)
{
	// copy name
	m_id = master.m_id;
	
	// copy components
	for( wxInt32 iter = 0; iter < GAME_COMP_COUNT; ++iter)
	{
		m_componentList[iter] = master.m_componentList[iter];
	}
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
	
	TCompSmPtr* endIter = &m_componentList[GAME_COMP_COUNT];
	for( TCompSmPtr* iter = m_componentList; iter != endIter; ++iter)
	{
		if(!iter->IsEmpty())
		{
			(*iter)->ReceiveMessage(msg);
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



