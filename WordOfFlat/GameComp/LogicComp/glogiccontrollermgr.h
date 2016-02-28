#ifndef __GAME_LOGIC_MOVEABLE_MANAGER_H__
#define __GAME_LOGIC_MOVEABLE_MANAGER_H__


#include <GameComp/gmanagerbase.h>
#include <GameComp/LogicComp/glogicsystem.h>
#include "glogiccontroller.h"



class LogicControllerManager : public GameManagerBase<LogicController>
{
private:
	GameLogicSystem* m_pLogicSystem;
public:
	LogicControllerManager(GameLogicSystem* pLogicSystem, GameEntityManager *pEntityMgr);
	~LogicControllerManager();
	
	GameErrorCode AddLogicController(wxDword compId, LogicController *pLogicController )
	{
		FWG_RETURN_FAIL(InsertToMap(compId, pLogicController));
		
		// add component to process
		if(pLogicController->IsEnabled())
		{
			FWG_RETURN_FAIL(m_pLogicSystem->AddLogicComp(pLogicController));
		} else {
			pLogicController->SetLogicSystem(m_pLogicSystem);
		}
		
		return FWG_NO_ERROR;
	}

};

#endif // __GAME_LOGIC_MOVEABLE_MANAGER_H__
