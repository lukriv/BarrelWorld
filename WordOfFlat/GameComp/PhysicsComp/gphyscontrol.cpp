#include "gphyscontrol.h"

PhysicsControler::PhysicsControler(GamePhysicsSystem* pPhysSystem) : ComponentBase(	GAME_COMP_PHYSICS_CONTROLER )
{
}

PhysicsControler::~PhysicsControler()
{
}

GameErrorCode PhysicsControler::Initialize(TransformComponent* pTransform)
{
	if(pTransform == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform.Attach(pTransform);
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsControler::Load(wxXmlNode* pNode)
{
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsControler::Store(wxXmlNode* pParentNode)
{
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsControler::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}

void PhysicsControler::debugDraw(btIDebugDraw* debugDrawer)
{
}

void PhysicsControler::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
}
