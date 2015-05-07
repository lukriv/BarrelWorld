#include "gphysstaticobj.h"

PhysicsStaticObject::PhysicsStaticObject() : ComponentBase(GAME_COMP_PHYSICS_STATIC_OBJECT)
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

GameErrorCode PhysicsStaticObject::Create()
{
	m_pColObject = new btCollisionObject;
	btTransform transform;
	pTrans->getWorldTransform(transform);
	m_pColObject->setWorldTransform(transform);
	m_pColObject->setCollisionShape(pColShape);
	
	
	
	return FWG_NO_ERROR;
}


