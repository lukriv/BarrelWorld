#include "gentity.h"



GameErrorCode GameEntity::AddComponent(ComponentBase* pComp)
{
	RefObjSmPtr<ComponentBase> spComponent(pComp);
	
	if(!pComp)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	return m_componentList.Insert(pComp->GetComponentType(), spComponent);
}

ComponentBase* GameEntity::GetComponent(GameComponentType compType)
{
	RefObjSmPtr<ComponentBase> *spComp = m_componentList.FindValue(compType);
	if(spComp)
	{
		return spComp->In();
	}
	
	return nullptr;
}

GameErrorCode GameEntity::ReceiveMessage(TaskMessage& msg, GameComponentType targetMask)
{
	TEntityComponentMap::Iterator iter;
	for( iter = m_componentList.Begin(); iter != m_componentList.End(); ++iter)
	{
		if((targetMask & iter->second->GetComponentType()) != 0)
		{
			iter->second->ReceiveMessage(msg);
		}
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntity::ReinitComponents()
{
	TEntityComponentMap::Iterator iter;
	for( iter = m_componentList.Begin(); iter != m_componentList.End(); ++iter)
	{
		FWG_RETURN_FAIL(iter->second->ReinitComponent(this));
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntity::RemoveComponent(GameComponentType compType)
{
	m_componentList.Remove(compType);
	return FWG_NO_ERROR;
}

GameErrorCode GameEntity::Update()
{
	TEntityComponentMap::Iterator iter;
	for( iter = m_componentList.Begin(); iter != m_componentList.End(); ++iter)
	{
		FWG_RETURN_FAIL(iter->second->Update());
	}
	
	return FWG_NO_ERROR;
}
