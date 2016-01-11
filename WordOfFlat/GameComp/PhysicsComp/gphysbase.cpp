#include "gphysbase.h"

#include "../gcompdefs.h"

PhysicsBase::PhysicsBase(GamePhysicsSystem* pPhysSystem) : ComponentBase(GAME_COMP_PHYSICS), m_pPhysSystem(pPhysSystem)
{}

PhysicsBase::~PhysicsBase()
{
}

GameErrorCode PhysicsBase::Initialize(TransformComponent* pTransform)
{
	if(pTransform == nullptr)
	{
		FWGLOG_ERROR(wxT("Transform component is null"), m_pPhysSystem->GetLogger());
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	
	return FWG_NO_ERROR;
}