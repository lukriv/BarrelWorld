#include "glogicfreecammgr.h"

LogicFreeCameraManager::LogicFreeCameraManager(GameLogger *pLogger) : GameManagerBase(pLogger)
{
}

LogicFreeCameraManager::~LogicFreeCameraManager()
{
}

GameErrorCode LogicFreeCameraManager::CreateComponent(wxDword compId, LogicFreeCamera*& pNewComponent)
{
	RefObjSmPtr<LogicFreeCamera> spInputComp;
	FWG_RETURN_FAIL(GameNewChecked(spInputComp.OutRef()));
	FWG_RETURN_FAIL(spInputComp->Initialize(m_pInputSystem));
	
	FWG_RETURN_FAIL(InsertToMap(compId, spInputComp));
	
	return FWG_NO_ERROR;
}
