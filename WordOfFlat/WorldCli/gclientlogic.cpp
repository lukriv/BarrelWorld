#include "gclientlogic.h"

#include "gmenu.h"
#include <GameComp/RenderComp/grenderobj.h>

ClientGameLogic::ClientGameLogic() : GameThread(wxTHREAD_JOINABLE),
								m_pRenderWindow(NULL),
								m_stopRequest(false),
								m_isInitialized(false)
{
}

ClientGameLogic::~ClientGameLogic()
{
	StopGame();
	Uninitialize();
}

GameErrorCode ClientGameLogic::Initialize(GameLogger* pLogger, Ogre::RenderWindow* pWindow, Ogre::SceneManager* pSceneManager, GameInputSystem* pInputSystem, GameMenuSystem* pGameMenuRes)
{
	GameErrorCode result = FWG_NO_ERROR;
	if(m_isInitialized)
	{
		return FWG_NO_ERROR;
	}
	
	// window and scene manager must be defined
	if((pWindow == nullptr)
		||(pSceneManager == nullptr)
		||(pInputSystem == nullptr)
		||(pGameMenuRes == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_pLogger = pLogger;
	
	m_pRenderWindow = pWindow;

	FWG_RETURN_FAIL(GameNewChecked(m_spEntityFactory.OutRef()));
	
	if(FWG_FAILED(result = m_spEntityFactory->Initialize(pLogger, pInputSystem)))
	{
		FWGLOG_ERROR_FORMAT(wxT("ClientGameLogic::Initialize() : Entity Factory initialize failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL( GameNewChecked(m_spCompManager.OutRef(), pLogger));
	
	if(FWG_FAILED(result = m_spCompManager->Initialize(pSceneManager)))
	{
		FWGLOG_ERROR_FORMAT(wxT("ClientGameLogic::Initialize() : Component manager initialize failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_spInputSystem = pInputSystem;
	
	FWG_RETURN_FAIL(GameNewChecked(m_spGameMenus.OutRef(), pGameMenuRes));
	FWG_RETURN_FAIL(m_spGameMenus.In()->Initialize(pWindow, pSceneManager, this));

	FWGLOG_DEBUG(wxT("Seccesfuly initialized (GameLogic)"), m_pLogger);
	m_isInitialized = true;
	return result;
}

void ClientGameLogic::Uninitialize()
{
	FWGLOG_DEBUG(wxT("Seccesfuly uninitialized (GameLogic)"), m_pLogger);
	m_spGameMenus.Release();
	m_spEntityFactory.Release();
	m_spInputSystem.Release();
	if(!m_spCompManager.IsEmpty())
	{
		m_spCompManager->Uninitialize();	
	}
	m_spCompManager.Release();
	m_pLogger.Release();
	m_pRenderWindow = NULL;
	m_isInitialized = false;
}

GameErrorCode ClientGameLogic::LoadGame(GameDefinitionHolder& defHolder)
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	// create scene manager
	FWG_RETURN_FAIL(m_spEntityFactory.In()->CreateAllEntities(defHolder, *m_spCompManager));
	
	return result;
}

GameErrorCode ClientGameLogic::StartGame()
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	FWGLOG_INFO(wxT("Starting game (GameLogic)"), m_pLogger);
	
	{
		wxCriticalSectionLocker prepareLock(m_renderLocker);
	
		if(FWG_FAILED(result = PrepareCameras()))
		{
			FWGLOG_ERROR_FORMAT(wxT("prepare cameras failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return 0;
		} else {
			FWGLOG_DEBUG(wxT("Cameras were prepared"), m_pLogger);
		}
		
		if(FWG_FAILED(result = m_spGameMenus.In()->SetVisible(true, 0)))
		{
			FWGLOG_ERROR_FORMAT(wxT("prepare menus failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return 0;
		} else {
			FWGLOG_DEBUG(wxT("Menus were prepared"), m_pLogger);
		}
		
		if(FWG_FAILED(result = PrepareGlobalInput()))
		{
			FWGLOG_ERROR_FORMAT(wxT("prepare inputs failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return 0;
		} else {
			FWGLOG_DEBUG(wxT("Inputs were prepared"), m_pLogger);
		}
		
		if(FWG_FAILED(result = PrepareLights()))
		{
			FWGLOG_ERROR_FORMAT(wxT("prepare lights failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
			return 0;
		} else {
			FWGLOG_DEBUG(wxT("Lights were prepared"), m_pLogger);
		}
	}
	
	// create socket worker thread
	if (FWG_FAILED(result = this->Create())) {
		FWGLOG_ERROR_FORMAT(wxT("ClientGameLogic::StartGame() : Create worker thread failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWGLOG_INFO(wxT("Thread created"), m_pLogger);
	
	// run socket worker thread
	if (FWG_FAILED(result = this->Run())) {
		FWGLOG_ERROR_FORMAT(wxT("ClientGameLogic::StartGame() : Start thread failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWGLOG_INFO(wxT("Thread is running"), m_pLogger);
	
	return result;
}

GameErrorCode ClientGameLogic::StopGame()
{
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	return StopRequest();
}

GameErrorCode ClientGameLogic::StoreGame(GameDefinitionHolder& defHolder)
{
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode ClientGameLogic::StopRequest()
{
	m_stopRequest = true;
	Wait();
	return FWG_NO_ERROR;
}

// main logic thread functions
void* ClientGameLogic::Entry()
{
	GameErrorCode result = FWG_NO_ERROR;
	Ogre::Timer timer;
	bool stopRequest = false;
	unsigned long time = 0;
	
	while(!stopRequest) 
	{
		{
			timer.reset();
			
			//FWGLOG_TRACE(wxT("Still running"), m_pLogger);
			wxCriticalSectionLocker lock(m_renderLocker);
			stopRequest = m_stopRequest;
			if(FWG_FAILED(result = m_spCompManager.In()->GetLogicManager().ProcessLogicStep()))
			{
				FWGLOG_ERROR_FORMAT(wxT("Logic step failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
				result = FWG_NO_ERROR;
			}
			
			time = timer.getMilliseconds();
				
		}
		
		if(time < 17)
		{
			wxThread::Sleep(17 - time);
		}
	}
	
	return 0;
}

bool ClientGameLogic::IsStopped()
{
	wxCriticalSectionLocker lock(m_renderLocker);
	return m_stopRequest;
	//return (!IsAlive());
}

GameErrorCode ClientGameLogic::PrepareCameras()
{
	RefObjSmPtr<RenderObject> spCamera = m_spCompManager.In()->GetRenderManager().GetCamera(wxString(MAIN_CAMERA_NAME));
	if(spCamera.IsEmpty())
	{
		FWGLOG_ERROR(wxT("Fatal error: MainCamera was not found"), m_pLogger);
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
	
	Ogre::Camera* pOgreCam = spCamera.In()->GetCamera();

	m_spCompManager->GetRenderManager().GetOgreSceneManager()->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	// prepare viewports
	Ogre::Viewport *viewPort = m_pRenderWindow->addViewport(pOgreCam);
	viewPort->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));

	viewPort->setOverlaysEnabled(true);

	pOgreCam->setAspectRatio(Ogre::Real(viewPort->getActualWidth()) / Ogre::Real(viewPort->getActualHeight()));

	Ogre::Viewport *viewPort2 = m_pRenderWindow->addViewport(pOgreCam, 1, 0.6f, 0.1f, 0.3f, 0.3f);
	viewPort2->setBackgroundColour(Ogre::ColourValue(0.1f,0.1f,0.1f));

	viewPort2->setOverlaysEnabled(false);
	
	return FWG_NO_ERROR;
}


GameErrorCode ClientGameLogic::PrepareGlobalInput()
{
	GameErrorCode result = FWG_NO_ERROR;

	if(FWG_FAILED(result = m_spInputSystem->RegisterCallback(OIS::KC_ESCAPE, this, &ClientGameLogic::SetExit))) {
		FWGLOG_ERROR_FORMAT(wxT("Register input callback failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	return result;
}

GameErrorCode ClientGameLogic::PrepareLights()
{
	GameErrorCode result = FWG_NO_ERROR;

	// Create a Light and set its position
	Ogre::Light* light = m_spCompManager->GetRenderManager().GetOgreSceneManager()->createLight("MainLight");
	light->setPosition(20.0f, 80.0f, 50.0f);


	return result;
}

void ClientGameLogic::SetExit(bool keyDown)
{
	wxCriticalSectionLocker lock(m_renderLocker);
	m_stopRequest = true; 
}
void ClientGameLogic::SetExitMenu(MyGUI::Widget* _sender)
{
	wxCriticalSectionLocker lock(m_renderLocker);
	m_stopRequest = true; 
}

void ClientGameLogic::SetShowExit(MyGUI::Widget* _sender)
{
	wxCriticalSectionLocker lock(m_renderLocker);
	m_spGameMenus.In()->SwitchExitButton();
}

