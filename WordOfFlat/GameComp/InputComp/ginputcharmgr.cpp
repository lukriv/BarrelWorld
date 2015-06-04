#include "ginputcharmgr.h"

InputCharacterManager::InputCharacterManager(GameInputSystem* pInputSystem) : GameManagerBase(pInputSystem->GetLogger()),
																			m_pInputSystem(pInputSystem)
{
}

InputCharacterManager::~InputCharacterManager()
{
}


GameErrorCode InputCharacterManager::CreateComponent(wxDword compId, CharacterInput*& pNewInputComp)
{
	RefObjSmPtr<CharacterInput> spInputComp;
	FWG_RETURN_FAIL(GameNewChecked(spInputComp.OutRef()));
	FWG_RETURN_FAIL(spInputComp->Initialize(m_pInputSystem));
	
	FWG_RETURN_FAIL(InsertToMap(compId, spInputComp));
	
	return FWG_NO_ERROR;
}
