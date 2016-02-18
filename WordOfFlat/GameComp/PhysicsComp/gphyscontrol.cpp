#include "gphyscontrol.h"

PhysicsController::PhysicsController(GamePhysicsSystem* pPhysSystem) : ComponentBase(	GAME_COMP_PHYSICS_CONTROLER )
{
}

PhysicsController::~PhysicsController()
{
}

GameErrorCode PhysicsController::Initialize(TransformComponent* pTransform)
{
	if(pTransform == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform.Attach(pTransform);
	
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
