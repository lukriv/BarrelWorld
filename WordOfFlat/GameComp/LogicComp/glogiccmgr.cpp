#include "glogiccmgr.h"

#include "glogiccomp.h"

LogicCompManager::LogicCompManager(GameLogger *pLogger) : m_spLogger(pLogger)
{
}

LogicCompManager::~LogicCompManager()
{}

GameErrorCode LogicCompManager::AddLogicComp(LogicComponentBase* pLogicComp)
{
	FWG_RETURN_FAIL(m_logicCompList.Insert(pLogicComp));
	pLogicComp->SetOwnerManager(this);
	return FWG_NO_ERROR;
}

GameErrorCode LogicCompManager::ProcessLogicStep()
{
	GameErrorCode result = FWG_NO_ERROR;
	for(auto iter = m_logicCompList.Begin(); iter != m_logicCompList.End(); iter++)
	{
		if(FWG_FAILED(result = (*iter)->ProcessLogic()))
		{
			FWGLOG_ERROR_FORMAT(wxT("ProcessLogic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		}
	}
	
	return FWG_NO_ERROR;
}

void LogicCompManager::RemoveLogicComp(LogicComponentBase* pLogicComp)
{
	pLogicComp->SetOwnerManager(nullptr);
	m_logicCompList.Remove(pLogicComp);
}
