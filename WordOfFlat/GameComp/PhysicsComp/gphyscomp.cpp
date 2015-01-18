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

GameErrorCode PhysicsComponent::InitializeInternal(GameEntity* pParentEntity, btCollisionObject * pColObj)
{
	if((pParentEntity == nullptr)||(pColObj == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_pParent = pParentEntity;
	
	m_pColObject = pColObj;
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsComponent::Initialize(GameEntity* pParentEntity, btCollisionObject* pColObj)
{
	FWG_RETURN_FAIL(InitializeInternal(pParentEntity, pColObj));
	
	m_pOwnerMgr->GetDynamicsWorld()->addCollisionObject(m_pColObject);
	
	m_type = PhysicsComponent::PHYS_COMP_TYPE_COLLISION;
	
	return FWG_NO_ERROR;	
}

GameErrorCode PhysicsComponent::Initialize(GameEntity* pParentEntity, btRigidBody* pColObj)
{
	FWG_RETURN_FAIL(InitializeInternal(pParentEntity, pColObj));
	
	m_pOwnerMgr->GetDynamicsWorld()->addRigidBody(pColObj);
	
	m_type = PhysicsComponent::PHYS_COMP_TYPE_RIGID;
	
	return FWG_NO_ERROR;
}

PhysicsComponent::~PhysicsComponent()
{
}

