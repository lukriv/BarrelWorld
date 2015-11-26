#include "ginputfreecam.h"

static const wxChar* FACTORY_INPUT_UP		 = wxT("up");
static const wxChar* FACTORY_INPUT_DOWN		 = wxT("down");
static const wxChar* FACTORY_INPUT_LEFT		 = wxT("left");
static const wxChar* FACTORY_INPUT_RIGHT	 = wxT("right");
static const wxChar* FACTORY_INPUT_FORWARD	 = wxT("forward");
static const wxChar* FACTORY_INPUT_BACKWARD	 = wxT("backward");

GameErrorCode FreeCameraInput::Create( InputDef *inputDef)
{
	
	GameErrorCode result = FWG_NO_ERROR;
	
	SetDefinition(inputDef);
	
	for(InputDef::TInputMap::ConstIterator iter = inputDef->m_inputMap.Begin();
		iter != inputDef->m_inputMap.End();
		++iter)
	{
	
		if(iter->first.Cmp(FACTORY_INPUT_UP) == 0)
		{
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &FreeCameraInput::SetMoveUp))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveUp failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_DOWN) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &FreeCameraInput::SetMoveDown))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveDown failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_LEFT) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &FreeCameraInput::SetMoveLeft))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveLeft failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_RIGHT) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &FreeCameraInput::SetMoveRight))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveRight failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_FORWARD) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &FreeCameraInput::SetMoveForward))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveForward failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else if (iter->first.Cmp(FACTORY_INPUT_BACKWARD) == 0) {
			if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(static_cast<OIS::KeyCode>(iter->second)
																	, this
																	, &FreeCameraInput::SetMoveBackward))) 
			{
				FWGLOG_ERROR_FORMAT(wxT("Register input callback moveBackward failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			FWGLOG_WARNING_FORMAT(wxT("Cannot register unknown input action '%s'"), m_pInputSystem->GetLogger(), iter->first.GetData().AsInternal(), FWGLOG_ENDVAL);
		}
	}
	
	if(FWG_FAILED(result = m_pInputSystem->RegisterCallback(this)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Register mouse callback failed: 0x%08x"), m_pInputSystem->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}


FreeCameraInput::~FreeCameraInput()
{
	SetDefinition(nullptr);
}


