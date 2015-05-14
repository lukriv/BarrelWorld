#include "gphysstaticobj.h"

#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>

#include "gphyscmgr.h"
#include "../transformComp/gtranscomp.h"

PhysicsStaticObject::PhysicsStaticObject(PhysicsCompManager* pOwnerMgr) : ComponentBase(GAME_COMP_PHYSICS_STATIC_OBJECT)
				, m_pOwnerMgr(pOwnerMgr)
				, m_pColObject(nullptr)
{}




PhysicsStaticObject::~PhysicsStaticObject()
{
	if(m_pColObject)
	{
		m_pColObject->setUserPointer(nullptr);
		m_pOwnerMgr->GetDynamicsWorld()->removeCollisionObject(m_pColObject);
		delete m_pColObject;
		m_pColObject = nullptr;
	}
}


GameErrorCode PhysicsStaticObject::Initialize(TransformComponent* pTransform)
{
	if (pTransform == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	
	return FWG_NO_ERROR;
}


GameErrorCode PhysicsStaticObject::Create(btCollisionShape *pColShape)
{
	if(m_spTransform.IsEmpty())
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	m_pColObject = new btCollisionObject;
	btTransform transform;
	m_spTransform->getWorldTransform(transform);
	m_pColObject->setWorldTransform(transform);
	m_pColObject->setCollisionShape(pColShape);
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsStaticObject::Update()
{
	return FWG_NO_ERROR;
}


GameErrorCode PhysicsStaticObject::ReceiveMessage(TaskMessage& msg)
{
	switch(msg.GetTaskType())
	{
		case GAME_TASK_TRANSFORM_UPDATE:
		{
			btTransform transform;
			m_spTransform->getWorldTransform(transform);
			m_pColObject->setWorldTransform(transform);
			//FWGLOG_DEBUG_FORMAT(wxT("Update physics for entity: %s"), m_pOwnerMgr->GetLogger(),
			//													m_pParent->GetName().GetData().AsInternal(),
			//													FWGLOG_ENDVAL);
			break;
		}
		default: 
			break;
	}
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsStaticObject::Load(wxXmlNode* XMLNode)
{
}

GameErrorCode PhysicsStaticObject::Store(wxXmlNode* ParentNode)
{
}
