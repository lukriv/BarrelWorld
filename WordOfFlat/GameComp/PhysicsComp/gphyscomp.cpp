#include "gphyscomp.h"

#include "gphyscmgr.h"

GameErrorCode PhysicsComponent::ReceiveMessage(TaskMessage& msg)
{
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode PhysicsComponent::ReinitComponent(GameEntity* pNewParentEntity)
{
	m_pParent = pNewParentEntity;
	// TODO: change MotionState object
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsComponent::Update()
{
	//btBoxShape
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsComponent::Initialize(GameEntity* pParentEntity, btRigidBody *pColObj)
{
	if((pParentEntity == nullptr)||(pColObj == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_pParent = pParentEntity;
	m_pColObject = pColObj;
	
	m_pOwnerMgr->GetDynamicsWorld()->addCollisionObject(m_pColObject);
	//m_pOwnerMgr->GetDynamicsWorld()->addRigidBody(m_pColObject);
	
	return FWG_NO_ERROR;
}

PhysicsComponent::~PhysicsComponent()
{
}
