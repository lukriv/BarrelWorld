#include "glogicfreecammgr.h"

LogicFreeCameraManager::LogicFreeCameraManager(GameInputSystem* pInputSystem) : GameManagerBase(pInputSystem->GetLogger())
																				, m_pInputSystem(pInputSystem)
{
}

LogicFreeCameraManager::~LogicFreeCameraManager()
{
}

GameErrorCode LogicFreeCameraManager::CreateComponent(wxDword compId, LogicFreeCamera*& pNewComponent)
{
	RefObjSmPtr<LogicFreeCamera> spLogicComp;
	FWG_RETURN_FAIL(GameNewChecked(spLogicComp.OutRef()));
	
	FWG_RETURN_FAIL(InsertToMap(compId, spLogicComp));
	
	return FWG_NO_ERROR;
}
