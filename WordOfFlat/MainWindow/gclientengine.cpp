#include "gclientengine.h"

static const wxChar* TESTING_TITLE = wxT("Flat World Client");

GameErrorCode GameClientEngine::CreateWindow()
{
	// Create the main window
	m_renderWindow = new (std::nothrow) sf::RenderWindow(
							sf::VideoMode(m_settings.m_screenWidth, m_settings.m_screenHeight),
							m_settings.m_screenTitle);
	if (m_renderWindow == nullptr)
	{
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateWindow() : Create window failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	

}

GameErrorCode GameClientEngine::Initialize(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (m_isInitialized)
	{
		return FWG_NO_ERROR;
	}
	
	m_pLogger = pLogger;
	
	if(FWG_FAILED(result = LoadSettings()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Load setting failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = CreateWindow()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Create window failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = GameResourceHolder::InitializeResourceHolder(pLogger))) 
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Initialize resource holder failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_spResHolder.Attach(GameResourceHolder::GetResourceHolder());
	
	m_pSceneGenerator = new (std::nothrow) GameTestSceneGenerator();
	if (m_pSceneGenerator == NULL) 
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_pEntityFactory = new (std::nothrow) GameEntityFactory(m_spResHolder.In());
	if (m_pEntityFactory == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
	
	m_isInitialized = false;
	return result;
}

GameErrorCode GameClientEngine::LoadSettings(wxChar* pFileName)
{
	if(pFileName == nullptr)
	{
		m_screenWidth = 800;
		m_screenHeight = 600;
		m_screenTitle = TESTING_TITLE;
		m_worldName = nullptr;
	} else {
		return FWG_E_NOT_IMPLEMENTED_ERROR;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameClientEngine::MainLoop()
{
	GameErrorCode result = FWG_NO_ERROR;
	      // Start the game loop
	while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
		 
		if (FWG_FAILED(result = m_pActualFlatWorldClient->DrawStep()))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::MainLoop() : DrawStep failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if (FWG_FAILED(result = m_pActualFlatWorldClient->SimulationStep()))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::MainLoop() : SimulationStep failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}

    }
}



GameErrorCode GameClientEngine::CreateTestingWorld()
{
	GameErrorCode result = FWG_NO_ERROR;
	sf::Texture* pTexture = NULL;
	sf::Shape* pShape = NULL;
	GameEntityBase* pEntity = NULL;
	
	m_pActualFlatWorldClient = new (std::nothrow) GameFlatWorldClient();
	if(m_pActualFlatWorldClient == NULL)
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create flat world client failed: 0x%08x"),
			m_pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL)
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	
	if(FWG_FAILED(result = m_pActualFlatWorldClient->Initialize(pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Initialize flat world client failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// create scene
	wxVector<EntityDef> worldObjDefs;
	wxVector<EntityDef>::iterator entDefIter;
	
	if(FWG_FAILED(result = m_pSceneGenerator->GenLandscape(0, worldObjDefs)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Load entity definitions (landscape) failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = m_pSceneGenerator->GenMoveableObj(0, worldObjDefs)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Load entity definitions (moveable) failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	wxInt32 i = 0;
	for (entDefIter = worldObjDefs.begin(); entDefIter != worldObjDefs.end(); entDefIter++)
	{
		
		if(FWG_FAILED(result = m_pEntityFactory->CreateEntity(*entDefIter, *m_pActualFlatWorldClient->GetPhysWorld(), pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->AddEntity(i, pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Add entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
	}

	return result;
	
}


