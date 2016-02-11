#include "gmoveable.h"
#include <GameComp/LogicComp/glogicsystem.h>



GameErrorCode Moveable::Initialize(TransformComponent* pTransform, InputComponent* pInput)
{
	
	if((pTransform == nullptr)||(pInput == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	m_spInput = pInput;
	
	// add component to process
	if(IsEnabled())
	{
		m_pLogicSystem->AddLogicComp(this);
	}
		
	return FWG_NO_ERROR;
}

Moveable::Moveable(GameLogicSystem *pLogicSystem) : LogicBase(pLogicSystem), ComponentBase(GAME_COMP_MOVEABLE)
{}

Moveable::~Moveable()
{
}

void Moveable::Enable(bool enable)
{
	if(enable != IsEnabled())
	{
		if(enable)
		{
			m_pLogicSystem->AddLogicComp(this);	
		} else {
			m_pLogicSystem->RemoveLogicComp(this);
		}
		
		ComponentBase::Enable(enable);
	}
}
