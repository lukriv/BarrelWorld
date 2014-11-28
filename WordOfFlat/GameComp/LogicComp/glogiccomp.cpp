#include "glogiccomp.h"
#include "glogiccmgr.h"

GameErrorCode LogicComponentBase::PhysicsProcess()
{
	return FWG_NO_ERROR;
}

GameErrorCode LogicComponentBase::ProcessLogic()
{
	FWG_RETURN_FAIL(UserLogic());
	return FWG_NO_ERROR;
}

LogicComponentBase::~LogicComponentBase()
{
	if(m_pOwnerManager != nullptr)
	{
		m_pOwnerManager->RemoveLogicComp(this);
	}
}


