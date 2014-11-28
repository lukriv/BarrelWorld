#include "gphyscomp.h"

GameErrorCode PhysicsComponent::ReceiveMessage(TaskMessage& msg)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode PhysicsComponent::ReinitComponent(GameEntity* pNewParentEntity)
{
	m_pParent = pNewParentEntity;
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsComponent::Update()
{
	return FWG_NO_ERROR;
}

