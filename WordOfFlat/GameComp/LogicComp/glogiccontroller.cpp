#include "glogiccontroller.h"
#include <GameComp/LogicComp/glogicsystem.h>



LogicController::LogicController() : ComponentBase(GAME_COMP_MOVEABLE)
{}

LogicController::~LogicController()
{
}

void LogicController::Enable(bool enable)
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


GameErrorCode LogicController::Load(wxXmlNode* )
{
	return FWG_NO_ERROR;
}

GameErrorCode LogicController::Store(wxXmlNode* )
{
	return FWG_NO_ERROR;
}

GameErrorCode LogicController::ReceiveMessage(TaskMessage& )
{
	return FWG_NO_ERROR;
}
