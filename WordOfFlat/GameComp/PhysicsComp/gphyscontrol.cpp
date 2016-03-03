#include "gphyscontrol.h"

PhysicsController::PhysicsController() : ComponentBase(	GAME_COMP_PHYSICS_CONTROLER )
{
}

PhysicsController::~PhysicsController()
{
	if(m_pPhysSystem)
	{
		m_pPhysSystem->GetDynamicsWorld()->removeAction(this);
	}
}

GameErrorCode PhysicsController::Initialize(TransformComponent* pTransform, PhysicsBase *pPhysics)
{
	if(pTransform == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	m_spKinematic = pPhysics;
	
	// initialize rigidBody as Kinematics
	pPhysics->GetRigidBody()->setMassProps(0, btVector3(0,0,0));
	pPhysics->GetRigidBody()->setCollisionFlags(pPhysics->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	pPhysics->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsController::Load(wxXmlNode* pNode)
{
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsController::Store(wxXmlNode* pParentNode)
{
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsController::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}

void PhysicsController::debugDraw(btIDebugDraw* debugDrawer)
{
}

void PhysicsController::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
}

GameErrorCode PhysicsController::_initialize(GamePhysicsSystem* pPhysSystem)
{
	m_pPhysSystem = pPhysSystem;
	m_pPhysSystem->GetDynamicsWorld()->addAction(this);
	
	return FWG_NO_ERROR;
}

