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

GameErrorCode CharacterInput::CreateObject(const InputDef& inputDef, GameInputSystem& inputSystem, InputComponent*& pNewInputComp)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<CharacterInput> spInputComp;
	FWG_RETURN_FAIL(GameNewChecked(spInputComp.OutRef()));
	
	for(InputDef::TInputMap::ConstIterator iter = inputDef.m_inputMap.Begin();
		iter != inputDef.m_inputMap.End();
		++iter)
	{
	
		if(iter->first.Cmp(FACTORY_INPUT_UP) == 0)
		{
			if(FWG_FAILED(result = inputSystem.RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &CharacterInput::SetMoveUp))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveUp failed: 0x%08x"), inputSystem.GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_DOWN) == 0) {
			if(FWG_FAILED(result = inputSystem.RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &CharacterInput::SetMoveDown))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveDown failed: 0x%08x"), inputSystem.GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_LEFT) == 0) {
			if(FWG_FAILED(result = inputSystem.RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &CharacterInput::SetMoveLeft))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveLeft failed: 0x%08x"), inputSystem.GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_RIGHT) == 0) {
			if(FWG_FAILED(result = inputSystem.RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &CharacterInput::SetMoveRight))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveRight failed: 0x%08x"), inputSystem.GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_FORWARD) == 0) {
			if(FWG_FAILED(result = inputSystem.RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &CharacterInput::SetMoveForward))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveForward failed: 0x%08x"), inputSystem.GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_BACKWARD) == 0) {
			if(FWG_FAILED(result = inputSystem.RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, spInputComp.In()
																	, &CharacterInput::SetMoveBackward))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveBackward failed: 0x%08x"), inputSystem.GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			FWGLOG_WARNING_FORMAT(wxT("Cannot register unknown input action '%s'"), inputSystem.GetLogger(), iter->first.GetData().AsInternal(), FWGLOG_ENDVAL);
		}
	}
	
	pNewInputComp = spInputComp.Detach();
	
	return FWG_NO_ERROR;
}
