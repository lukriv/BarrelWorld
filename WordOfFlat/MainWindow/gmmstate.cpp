#include "gmmstate.h"
#include "GameXmlDefinitions/gxmlloader.h"

static const LogicStepTime = 1.0f / 60.0f;

m_spInputSystem->RegisterCallback(OIS::KC_ESCAPE, this, &ClientGameLogic::SetExit)))


GameErrorCode GameMainMenuState::ProcessUpdate(float secDiff)
{
	static float logicStep = LogicStepTime;
	
	GameErrorCode result = FWG_NO_ERROR;
	
	logicStep -= secDiff;
	if(logicStep < 0)
	{
		if(FWG_FAILED(result = m_spCompManager->GetRenderManager().ProcessLogicStep();))
		{
			FWGLOG_ERROR_FORMAT(wxT("Process logic steps failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
		
		logicStep += LogicStepTime;
	}
	
	if(FWG_FAILED(result = m_spCompManager->GetRenderManager().ProcessAllUpdates()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Process render updates failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	return FWG_NO_ERROR;
	
}



GameErrorCode GameMainMenuState::ProcessState(GameState& nextState, wxString& nextStateParams)
{
	if((!m_pOwner)||(!m_spCompManager.IsEmpty()))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	GameXmlResourceLoader loader;
	if(FWG_FAILED(result = loader.Initialize(wxT("example.xml"), wxT("res/"), m_pOwner->GetLogger())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Xml loader initialize failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = loader.Load(*m_spDefHolder)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load xml failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	GameDefinitionHolder defHolder;
	if(FWG_FAILED(result = m_pOwner->GetFactory()->CreateAllEntities(defHolder, *m_spCompManager)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create entities failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(m_spMenu.IsEmpty())
	{
		// create new menu
		FWG_RETURN_FAIL(GameNewChecked(m_spMenu.OutRef(), &m_spCompManager->GetMenuSystem(), m_pOwner->GetLogger()));
		FWG_RETURN_FAIL(GameNewChecked(m_pMenuClbk, this));
		if(FWG_FAILED(result = m_spMenu->Initialize(m_pMenuClbk)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Initialize menu failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
			return result;			
		}
	}
	
	FWG_RETURN_FAIL(m_spMenu->Show());
	
	m_spCompManager->GetRenderManager().GetOgreRoot()->addFrameListener(this);
	
	m_spCompManager->GetRenderManager().GetOgreRoot()->startRendering();
	
	m_spCompManager->GetRenderManager().GetOgreRoot()->removeFrameListener(this);
	
	nextState = m_nextState;
	nextStateParams = m_nextStateParams;
	
	return result;
}

bool GameMainMenuState::frameEnded(const Ogre::FrameEvent& )
{
	return true;
}

bool GameMainMenuState::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	
	GameErrorCode result = FWG_NO_ERROR;
	
	if(FWG_FAILED(result = m_spCompManager->GetInputSystem().ProcessInputs()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Process input failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return false;
	}

	if(FWG_FAILED(result = ProcessUpdate(evt.timeSinceLastFrame)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Process update failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return false;
	}
	
	return true;
}

bool GameMainMenuState::frameStarted(const Ogre::FrameEvent& )
{
	return true;
}


void GameMainMenuState::SetExitInputClbk(bool)
{
	//callbacks
	m_nextState = GAME_STATE_EXIT;
	m_exitState = true;
}

void GameMainMenuState::MenuCallback::OnExitEvent()
{
	m_pOwner->m_nextState = GAME_STATE_EXIT;
	m_pOwner->m_exitState = true;
}

