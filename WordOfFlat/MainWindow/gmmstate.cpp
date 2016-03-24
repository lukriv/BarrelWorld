#include "gmmstate.h"
#include <GameXmlDefinitions/gxmlloader.h>

#include <GameComp/PhysicsComp/gphysdbgdraw.h>


//m_spInputSystem->RegisterCallback(OIS::KC_ESCAPE, this, &ClientGameLogic::SetExit)))



GameErrorCode GameMainMenuState::ProcessUpdate(float secDiff)
{
	
	GameErrorCode result = FWG_NO_ERROR;
	
	//FWGLOG_DEBUG(wxT("Start physics steps"), m_pOwner->GetLogger());
	if(FWG_FAILED(result = m_spCompManager->GetPhysicsSystem().ProcessPhysics(secDiff)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Process physics steps failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
		//FWGLOG_DEBUG(wxT("Start logic and end physics steps"), m_pOwner->GetLogger());
		
	if(m_pDebugDraw)
	{
		m_pDebugDraw->Update();
	}

	if(FWG_FAILED(result = m_spCompManager->GetLogicSystem().ProcessLogicStep(secDiff)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Process logic steps failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
		
		
	
	if(FWG_FAILED(result = m_spCompManager->GetRenderSystem().ProcessAllUpdates()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Process render updates failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(m_switchPolyMode)
	{
		Ogre::Camera *pCamera = m_spCompManager->GetRenderSystem().GetMainCamera();
		Ogre::PolygonMode polyMode = pCamera->getPolygonMode();
		if(polyMode == Ogre::PM_SOLID)
		{
			pCamera->setPolygonMode(Ogre::PM_WIREFRAME);
		} else {
			pCamera->setPolygonMode(Ogre::PM_SOLID);
		}
		
		m_switchPolyMode = false;
	}
	
	return FWG_NO_ERROR;
	
}

GameErrorCode GameMainMenuState::LoadScene()
{
	GameErrorCode result = FWG_NO_ERROR;
	GameXmlResourceLoader loader;
	
	if(FWG_FAILED(result = loader.Initialize(wxT("example.xml"), wxT("res/"), m_pOwner->GetLogger())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Xml loader initialize failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWGLOG_INFO(wxT("Loader successfully initialized"), m_pOwner->GetLogger());
	
	if(FWG_FAILED(result = loader.Load(*m_spCompManager)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load xml failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}

	FWGLOG_INFO(wxT("Scene loaded"), m_pOwner->GetLogger());
	
	return FWG_NO_ERROR;
}

GameErrorCode GameMainMenuState::ProcessState(GameState& nextState, wxString& nextStateParams)
{
	if((!m_pOwner)||(m_spCompManager.IsEmpty()))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	GameErrorCode result = FWG_NO_ERROR;

	FWG_RETURN_FAIL(CreateScene());
	
	if(FWG_FAILED(result = m_spCompManager->GetRenderSystem().SetMainCamera("MainCamera")))
	{
		FWGLOG_ERROR_FORMAT(wxT("Set main camera failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWGLOG_INFO(wxT("Main camera set"), m_pOwner->GetLogger());
	
	// temporary light for menus
	m_spCompManager->GetRenderSystem().GetOgreSceneManager()->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));
	
	
	if(m_spMenu.IsEmpty())
	{
		// create new menu
		FWG_RETURN_FAIL(GameNewChecked(m_spMenu.OutRef(), &m_spCompManager->GetMenuSystem(), &m_spCompManager->GetInputSystem(), m_pOwner->GetLogger()));
		FWG_RETURN_FAIL(GameNewChecked(m_pMenuClbk, this));
		if(FWG_FAILED(result = m_spMenu->Initialize(m_pMenuClbk)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Initialize menu failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
			return result;			
		}
		
	}
	
	// register key callback
	if(FWG_FAILED(result = m_spCompManager->GetInputSystem().RegisterCallback(OIS::KC_ESCAPE, this, &GameMainMenuState::SetExitInputClbk)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Register input callback failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL(m_spMenu->Show());
	
	m_spCompManager->GetRenderSystem().GetOgreRoot()->addFrameListener(this);
	
	// physics debug draw
	//m_pDebugDraw = new CDebugDraw( m_spCompManager->GetRenderManager().GetOgreSceneManager(), m_spCompManager->GetPhysicsManager().GetDynamicsWorld());
	m_spCompManager->GetRenderSystem().GetOgreRoot()->startRendering();
	
	if(m_pDebugDraw)
	{
		delete m_pDebugDraw;
		m_pDebugDraw = nullptr;
	}
 	
	m_spCompManager->GetRenderSystem().GetOgreRoot()->removeFrameListener(this);
	
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
	
	// exit
	if(m_exitState)
	{
		return false;
	}
	
	return true;
}

bool GameMainMenuState::frameStarted(const Ogre::FrameEvent& )
{
	return true;
}


void GameMainMenuState::SetExitInputClbk(bool pressed)
{
	//callbacks
	if(pressed)
	{
		m_mainMenu = !m_mainMenu;
		m_spMenu->ShowMenuButtons(m_mainMenu);
	}
}

void GameMainMenuState::SwitchPhysicsDebug()
{
	if(!m_pDebugDraw)
	{
		m_pDebugDraw = new CDebugDraw( m_spCompManager->GetRenderSystem().GetOgreSceneManager(), m_spCompManager->GetPhysicsSystem().GetDynamicsWorld());
	} else {
		delete m_pDebugDraw;
		m_pDebugDraw = nullptr;
	}
}

GameErrorCode GameMainMenuState::CreateScene()
{
	GameErrorCode result = FWG_NO_ERROR;
	GameEntityFactory factory(m_pOwner->GetLogger());
	
	if(FWG_FAILED(result = factory.CreateFloor(*m_spCompManager)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create floor failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = factory.CreateMainCamera(*m_spCompManager, m_cameraId)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create main camera failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	wxDword boxId;
	if(FWG_FAILED(result = factory.CreateBox(*m_spCompManager, boxId)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create test box failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = factory.CreateStaticBox(*m_spCompManager, boxId)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create test box failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = factory.CreateAvatar(*m_spCompManager, m_avatarId)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create avatar failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	TaskMessage msg(GAME_TASK_FOLLOW_OBJECT, reinterpret_cast<void*>(m_avatarId));
	if(FWG_FAILED(result = m_spCompManager->GetEntityManager().SendTaskMessage(m_cameraId, msg)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Send message to camera failed: 0x%08x"), m_pOwner->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}



void GameMainMenuState::MenuCallback::OnExitEvent()
{
	m_pOwner->m_nextState = GAME_STATE_EXIT;
	m_pOwner->m_exitState = true;
}

void GameMainMenuState::MenuCallback::OnSwitchEvent()
{
	m_pOwner->m_switchPolyMode = true;
}

void GameMainMenuState::MenuCallback::OnDebugEvent()
{
	m_pOwner->SwitchPhysicsDebug();
}
