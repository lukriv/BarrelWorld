#include "gclientlogic.h"

ClientGameLogic::ClientGameLogic() : m_stopped(false),
								m_stopRequest(false),
								m_isInitialized(false)
{
}

ClientGameLogic::~ClientGameLogic()
{
	Uninitialize();
}

GameErrorCode ClientGameLogic::Initialize(GameLogger* pLogger, Ogre::RenderWindow* pWindow, Ogre::SceneManager* pSceneManager, GameInputSystem* pInputSystem, GameMenu* pGameMenu)
{
	GameErrorCode result = FWG_NO_ERROR;
	if(m_isInitialized)
	{
		return FWG_NO_ERROR;
	}
	
	// window and scene manager must be defined
	if((pWindow == NULL)||(pSceneManager == NULL))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_pLogger = pLogger;

	FWG_RETURN_FAIL(GameNewChecked(m_spEntityFactory.OutRef()));
	
	if(FWG_FAILED(result = m_spEntityFactory->Initialize(pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("ClientGameLogic::Initialize() : Entity Factory initialize failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	FWG_RETURN_FAIL( GameNewChecked(m_spComponentManager.OutRef()));
	
	if(FWG_FAILED(result = m_spComponentManager->Initialize(pSceneManager)))
	{
		FWGLOG_ERROR_FORMAT(wxT("ClientGameLogic::Initialize() : Component manager initialize failed: 0x%08x"), m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_spInputSystem = pInputSystem;
	m_spGameMenus = pGameMenu;

	m_isInitialized = true;
	return result;
}

void ClientGameLogic::Uninitialize()
{
	m_spCompManager.Release();
	m_spEntityFactory.Release();
	m_spInputSystem.Release();
	m_spGameMenus.Release();
	m_pLogger.Release();
}

GameErrorCode ClientGameLogic::LoadGame(GameDefinitionHolder& defHolder)
{
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	// create scene manager
	FWG_RETURN_FAIL(m_spEntityFactory.In()->CreateAllEntities(defHolder, m_spComponentManager));
	
	return result;
}

GameErrorCode ClientGameLogic::StartGame()
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	// create socket worker thread
	if (FWG_FAILED(result = this->Create())) {
		FWGLOG_ERROR_FORMAT(wxT("ClientGameLogic::StartGame() : Create worker thread failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// run socket worker thread
	if (FWG_FAILED(result = this->Run())) {
		FWGLOG_ERROR_FORMAT(wxT("ClientGameLogic::StartGame() : Start thread failed: 0x%08x"),
						m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	
	return result;
}

GameErrorCode ClientGameLogic::StopGame()
{
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	return FWG_NO_ERROR;
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
	
	
	PrepareCameras();
	PrepareLights();
	m_spGameMenus.In()->PrepareIngameMenu(this);
	
	while(!m_stopRequest) 
	{
		wxThread::Sleep(17);
	}
	
	m_stopped = true; // set stopped flag
	return 0;
}


