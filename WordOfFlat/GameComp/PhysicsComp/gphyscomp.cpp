#include "gphyscomp.h"

#include "gphyscmgr.h"
#include <GameComp/transformComp/gtranscomp.h>
#include <GameComp/gentity.h>

GameErrorCode PhysicsComponent::ReceiveMessage(TaskMessage& msg)
{
	switch(msg.GetTaskType())
	{
		case GAME_TASK_TRANSFORM_UPDATE:
		{
			//if(m_type == PHYS_COMP_TYPE_COLLISION)
			//{
				RefObjSmPtr<TransformComponent> spTransform = reinterpret_cast<TransformComponent*>(m_pParent->GetComponent(GAME_COMP_TRANSFORM));
				btTransform transform;
				spTransform->getWorldTransform(transform);
				m_pColObject->setWorldTransform(transform);
			//FWGLOG_DEBUG_FORMAT(wxT("Update physics for entity: %s"), m_pOwnerMgr->GetLogger(),
			//													m_pParent->GetName().GetData().AsInternal(),
			//													FWGLOG_ENDVAL);
			//}
			break;
		}
		default: 
			break;
	}
	return FWG_NO_ERROR;
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
	
	// set pointer to this object
	m_pColObject->setUserPointer(reinterpret_cast<void*>(this));
	
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
	if(m_pColObject)
	{
		m_pColObject->setUserPointer(nullptr);
		switch(m_type)
		{
			case PHYS_COMP_TYPE_RIGID:
				m_pOwnerMgr->GetDynamicsWorld()->removeRigidBody(reinterpret_cast<btRigidBody*>(m_pColObject));
				break;
			case PHYS_COMP_TYPE_COLLISION:
				m_pOwnerMgr->GetDynamicsWorld()->removeCollisionObject(m_pColObject);
				break;
			default:
				break;
		}
		delete m_pColObject;
		m_pColObject = nullptr;
	}
}

