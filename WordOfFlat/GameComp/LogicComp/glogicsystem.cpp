#include "glogicsystem.h"

#include "glogicbase.h"
#include "../gcompmgr.h"

GameLogicSystem::GameLogicSystem(GameLogger *pLogger) : m_spLogger(pLogger)
{
}

GameLogicSystem::~GameLogicSystem()
{}

GameErrorCode GameLogicSystem::AddLogicComp(LogicBase* pLogicComp)
{
	wxCriticalSectionLocker lock( m_lockMgr );
	FWG_RETURN_FAIL(m_logicCompList.Insert(pLogicComp));
	return FWG_NO_ERROR;
}

GameErrorCode GameLogicSystem::ProcessLogicStep(float timeDiff)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxCriticalSectionLocker lock(m_lockMgr);
	for(auto iter = m_logicCompList.Begin(); iter != m_logicCompList.End(); ++iter)
	{
		if(FWG_FAILED(result = (*iter)->Update(timeDiff)))
		{
			FWGLOG_ERROR_FORMAT(wxT("ProcessLogic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		}
	}
	
	return FWG_NO_ERROR;
}

void GameLogicSystem::RemoveLogicComp(LogicBase* pLogicComp)
{
	wxCriticalSectionLocker lock(m_lockMgr);
	m_logicCompList.Remove(pLogicComp);
}

GameErrorCode GameLogicSystem::Initialize()
{
	
	return FWG_NO_ERROR;
}
