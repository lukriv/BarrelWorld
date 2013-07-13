#include "gclientengine.h"
#include "../GameResHold/gscenegenimpl.h"



static const wxChar* TESTING_TITLE = wxT("Flat World Client");

GameErrorCode GameClientEngine::CreateWindow()
{
	GameErrorCode result = FWG_NO_ERROR;
	// Create the main window
	m_renderWindow = new (std::nothrow) sf::RenderWindow(
							sf::VideoMode(m_settings.m_screenWidth, m_settings.m_screenHeight),
							m_settings.m_screenTitle.GetData().AsWChar());
	if (m_renderWindow == NULL)
	{
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateWindow() : Create window failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	
	return result;
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
	if(pFileName == NULL)
	{
		m_settings.m_screenWidth = 800;
		m_settings.m_screenHeight = 600;
		m_settings.m_screenTitle.assign(TESTING_TITLE);
		m_settings.m_worldName.Clear();
	} else {
		return FWG_E_NOT_IMPLEMENTED_ERROR;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameClientEngine::MainLoop()
{
	GameErrorCode result = FWG_NO_ERROR;
	      // Start the game loop
	while (m_renderWindow->isOpen())
    {
        // Process events
        sf::Event event;
        while (m_renderWindow->pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                m_renderWindow->close();
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
	
	return result;
}



GameErrorCode GameClientEngine::CreateTestingWorld()
{
	GameErrorCode result = FWG_NO_ERROR;
	GameEntityBase* pEntity = NULL;
	wxInt32 i = 0;
	m_pActualFlatWorldClient = new (std::nothrow) GameFlatWorldClient();
	if(m_pActualFlatWorldClient == NULL)
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create flat world client failed: 0x%08x"),
			m_pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	
	if(FWG_FAILED(result = m_pActualFlatWorldClient->Initialize(m_renderWindow, m_pLogger.In())))
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
	
	i = 0;
	for (entDefIter = worldObjDefs.begin(); entDefIter != worldObjDefs.end(); entDefIter++)
	{
		
		if(FWG_FAILED(result = m_pEntityFactory->CreateEntity(*entDefIter, *m_pActualFlatWorldClient->GetPhysWorld(), pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->AddLandscapeEntity(i, pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Add entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
	}
	
	worldObjDefs.clear();
	if(FWG_FAILED(result = m_pSceneGenerator->GenMoveableObj(0, worldObjDefs)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Load entity definitions (moveable) failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	i = 0;
	for (entDefIter = worldObjDefs.begin(); entDefIter != worldObjDefs.end(); entDefIter++)
	{
		
		if(FWG_FAILED(result = m_pEntityFactory->CreateEntity(*entDefIter, *m_pActualFlatWorldClient->GetPhysWorld(), pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->AddMoveableEntity(i, pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Add entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
	}


	return result;
	
}


