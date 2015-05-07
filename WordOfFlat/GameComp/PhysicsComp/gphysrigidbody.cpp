#include "gphysrigidbody.h"


#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>

#include "../transformComp/gtranscomp.h"
#include "gphyscmgr.h"


PhysicsRigidBody::PhysicsRigidBody(PhysicsCompManager* pOwnerMgr)
    : ComponentBase(GAME_COMP_PHYSICS_RIGID_BODY)
    , m_pOwnerMgr(pOwnerMgr)
    , m_pRigidBody(nullptr)
{
}

PhysicsRigidBody::~PhysicsRigidBody()
{
	if(m_pRigidBody)
	{
		m_pRigidBody->setUserPointer(nullptr);
		m_pOwnerMgr->GetDynamicsWorld()->removeRigidBody(m_pRigidBody);
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}
}

GameErrorCode PhysicsRigidBody::Initialize(TransformComponent* pTransform)
{
	if(pTransform == nullptr)
	{
		FWGLOG_ERROR(wxT("Transform component is null"), m_pOwnerMgr->GetLogger());
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsRigidBody::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsRigidBody::Update()
{
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsRigidBody::Create(btScalar mass, btCollisionShape* pColShape)
{
	if(m_spTransform.IsEmpty())
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	btVector3 localInertia;
	localInertia.setZero();
	
	pColShape->calculateLocalInertia(mass, localInertia);
	
	btRigidBody::btRigidBodyConstructionInfo info(mass, m_spTransform.In(), pColShape, localInertia);
	
	m_pRigidBody = new btRigidBody(info);
	if(m_pRigidBody == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_pOwnerMgr->GetDynamicsWorld()->addRigidBody(m_pRigidBody);
	
	// set pointer to this object
	m_pRigidBody->setUserPointer(static_cast<void*>(this));
	
	return FWG_NO_ERROR;
	
}




