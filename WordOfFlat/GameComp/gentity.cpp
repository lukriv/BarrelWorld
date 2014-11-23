#include "gentity.h"



GameErrorCode GameEntity::AddComponent(ComponentBase* pComp)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<ComponentBase> spComponent(pComp);
	
	if(!pComp)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	return m_componentList->Insert(pComp->GetComponentType(), spComponent);
}

ComponentBase* GameEntity::GetComponent(GameComponentType compType)
{
	RefObjSmPtr<ComponentBase> *spComp = m_componentList->FindValue(compType);
	if(spComp)
	{
		return spComp.In();
	}
	
	return nullptr;
}

GameErrorCode GameEntity::ReceiveMessage(TaskMessage& msg, GameComponentType targetMask)
{
	TEntityComponentMap::Iterator iter;
	for( iter = m_componentList.Begin(); iter != m_componentList.End(); iter++)
	{
		if((targetMask & iter->second->GetComponentType()) != nullptr)
		{
			iter->second->ReceiveMessage(msg);
		}
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntity::ReinitComponents()
{
	TEntityComponentMap::Iterator iter;
	for( iter = m_componentList.Begin(); iter != m_componentList.End(); iter++)
	{
		iter->second->ReinitComponent(this);
	}
}

GameErrorCode GameEntity::RemoveComponent(ComponentBase* pComp)
{
}

GameErrorCode GameEntity::RemoveComponent(GameComponentType compType)
{
}

GameErrorCode GameEntity::Update()
{
}
