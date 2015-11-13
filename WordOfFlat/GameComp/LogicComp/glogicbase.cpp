#include "glogicbase.h"
#include "glogiccmgr.h"

GameErrorCode LogicBase::PhysicsProcess()
{
	return FWG_NO_ERROR;
}

GameErrorCode LogicBase::ProcessLogic()
{
	FWG_RETURN_FAIL(UserLogic());
	return FWG_NO_ERROR;
}

LogicBase::~LogicBase()
{
	if(m_pOwnerManager != nullptr)
	{
		m_pOwnerManager->RemoveLogicComp(this);
	}
}


