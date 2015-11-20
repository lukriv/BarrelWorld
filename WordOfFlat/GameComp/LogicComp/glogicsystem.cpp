#include "glogicsystem.h"

#include "glogicbase.h"
#include "../gcompmgr.h"

LogicSystem::LogicSystem(GameLogger *pLogger) : m_spLogger(pLogger)
												, m_pComponentManager(nullptr)
{
}

LogicSystem::~LogicSystem()
{}

GameErrorCode LogicSystem::AddLogicComp(LogicBase* pLogicComp)
{
	wxCriticalSectionLocker lock( m_lockMgr );
	FWG_RETURN_FAIL(m_logicCompList.Insert(pLogicComp));
	pLogicComp->SetOwnerManager(this);
	return FWG_NO_ERROR;
}

GameErrorCode LogicSystem::ProcessLogicStep()
{
	GameErrorCode result = FWG_NO_ERROR;
	wxCriticalSectionLocker lock(m_lockMgr);
	for(auto iter = m_logicCompList.Begin(); iter != m_logicCompList.End(); ++iter)
	{
		if(FWG_FAILED(result = (*iter)->ProcessLogic()))
		{
			FWGLOG_ERROR_FORMAT(wxT("ProcessLogic failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		}
	}
	
	return FWG_NO_ERROR;
}

void LogicSystem::RemoveLogicComp(LogicBase* pLogicComp)
{
	pLogicComp->SetOwnerManager(nullptr);
	
	wxCriticalSectionLocker lock(m_lockMgr);
	m_logicCompList.Remove(pLogicComp);
}

GameErrorCode LogicSystem::Initialize()
{
	
	return FWG_NO_ERROR;
}
