#include "ginputchar.h"

#include <OIS/OISKeyboard.h>
#include <GameComp/InputComp/ginputsystem.h>

/// create methods ///
static const wxChar* FACTORY_INPUT_UP		 = wxT("up");
static const wxChar* FACTORY_INPUT_DOWN		 = wxT("down");
static const wxChar* FACTORY_INPUT_LEFT		 = wxT("left");
static const wxChar* FACTORY_INPUT_RIGHT	 = wxT("right");
static const wxChar* FACTORY_INPUT_FORWARD	 = wxT("forward");
static const wxChar* FACTORY_INPUT_BACKWARD	 = wxT("backward");


CharacterInput::CharacterInput() : InputComponent(GAME_COMP_INPUT_CHARACTER), m_pInputSystem(nullptr)
{
}

GameErrorCode CharacterInput::Create(const InputDef& inputDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	for(InputDef::TInputMap::ConstIterator iter = inputDef.m_inputMap.Begin();
		iter != inputDef.m_inputMap.End();
		++iter)
	{
	
		if(iter->first.Cmp(FACTORY_INPUT_UP) == 0)
		{
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &CharacterInput::SetMoveUp))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveUp failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_DOWN) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &CharacterInput::SetMoveDown))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveDown failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_LEFT) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &CharacterInput::SetMoveLeft))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveLeft failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_RIGHT) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &CharacterInput::SetMoveRight))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveRight failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_FORWARD) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &CharacterInput::SetMoveForward))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveForward failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_BACKWARD) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &CharacterInput::SetMoveBackward))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveBackward failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			FWGLOG_WARNING_FORMAT(wxT("Cannot register unknown input action '%s'"), m_pInputSystem->GetLogger(), iter->first.GetData().AsInternal(), FWGLOG_ENDVAL);
		}
	}
		
	return FWG_NO_ERROR;
}


GameErrorCode CharacterInput::CreateObject(GameInputSystem* pInputSystem, CharacterInput &*pNewComponent)
{
	RefObjSmPtr<CharacterInput> spInputComp;
	FWG_RETURN_FAIL(GameNewChecked(spInputComp.OutRef()));
	FWG_RETURN_FAIL(spInputComp->Initialize(m_pInputSystem));
	pInputSystem = spInputComp.Detach();
	
	return FWG_NO_ERROR;
}
