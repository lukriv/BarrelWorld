#include "ginputfreecammgr.h"
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/InputComp/ginputsystem.h>
#include <GameComp/InputComp/ginputcomp.h>


InputFreeCamManager::InputFreeCamManager(GameInputSystem* pInputSystem) : GameManagerBase(pInputSystem->GetLogger())
																		, m_pInputSystem(pInputSystem)
{}

InputFreeCamManager::~InputFreeCamManager()
{
}

GameErrorCode InputFreeCamManager::CreateComponent(wxDword compId, FreeCameraInput*& pNewInputComp)
{
	RefObjSmPtr<FreeCameraInput> spInputComp;
	FWG_RETURN_FAIL(GameNewChecked(spInputComp.OutRef()));
	FWG_RETURN_FAIL(spInputComp->Initialize(m_pInputSystem));
	
	FWG_RETURN_FAIL(InsertToMap(compId, spInputComp));
	
	return FWG_NO_ERROR;
	
}


