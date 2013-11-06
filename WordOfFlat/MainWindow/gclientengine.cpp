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
	
	m_renderWindow->setVerticalSyncEnabled(true);
	
	
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
	
	m_spResHolder = GameResourceHolder::GetResourceHolder();
	
	m_pSceneGenerator = new (std::nothrow) GameTestSceneGenerator();
	if (m_pSceneGenerator == NULL) 
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_spEntityFactory.Attach(new (std::nothrow) GameEntityFactory());
	if (m_spEntityFactory == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
	
	if(FWG_FAILED(result = m_spEntityFactory->Initialize(m_spResHolder.In(), pLogger))) 
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Initialize entity factory failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
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
	sf::Clock clock;
	sf::Time time;
	// Start the game loop
	while (m_renderWindow->isOpen())
    {
        
		clock.restart();
		// Process events
        sf::Event event;
        while (m_renderWindow->pollEvent(event))
        {
            // Close window : exit
			switch (event.type)
			{
				case sf::Event::Closed:
					m_renderWindow->close();
					break;
				default:
					m_pActualFlatWorldClient->EventProcess(event);
					break;
			}
        }
		
		m_pActualFlatWorldClient->EventStep();
		
		m_renderWindow->clear(sf::Color(0,0,50));
		
		if (FWG_FAILED(result = m_pActualFlatWorldClient->DrawStep()))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::MainLoop() : DrawStep failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		m_renderWindow->display();
		
		if (FWG_FAILED(result = m_pActualFlatWorldClient->SimulationStep()))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::MainLoop() : SimulationStep failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		time = clock.getElapsedTime();
		if (time.asMilliseconds() < 16) {
			wxMilliSleep(16 - time.asMilliseconds());
		}

    }
	
	return result;
}



GameErrorCode GameClientEngine::CreateTestingWorld()
{
	GameErrorCode result = FWG_NO_ERROR;
	GameRenderComp* pEntity = NULL;
	
	m_pActualFlatWorldClient = new (std::nothrow) GameFlatWorldClient();
	if(m_pActualFlatWorldClient == NULL)
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create flat world client failed: 0x%08x"),
			m_pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	
	if(FWG_FAILED(result = m_pActualFlatWorldClient->Initialize(m_renderWindow, m_spEntityFactory.Ref(), m_pLogger.In())))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Initialize flat world client failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// create scene
	wxVector<RenderCompDef> worldObjDefs;
	wxVector<RenderCompDef>::iterator entDefIter;
	
	if(FWG_FAILED(result = m_pSceneGenerator->GenLandscape(0, worldObjDefs)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Load entity definitions (landscape) failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	for (entDefIter = worldObjDefs.begin(); entDefIter != worldObjDefs.end(); entDefIter++)
	{
		if(FWG_FAILED(result = m_pActualFlatWorldClient->CreateNewEntity(pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create new entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
		}
		
		if(FWG_FAILED(result = m_spEntityFactory->CreateBasicEntity(*entDefIter, *m_pActualFlatWorldClient->GetPhysWorld(), *pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create entity in factory failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->AddLandscapeEntity(pEntity)))
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
	
	for (entDefIter = worldObjDefs.begin(); entDefIter != worldObjDefs.end(); entDefIter++)
	{
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->CreateNewEntity(pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create new entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_spEntityFactory->CreateBasicEntity(*entDefIter, *m_pActualFlatWorldClient->GetPhysWorld(), *pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->AddMoveableEntity(pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Add entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
	}
	
	{
		GameAnimation *pAnimation = NULL;
		GameAnimation *pAnimation2 = NULL;
		AnimationDef animdef;
		RenderCompDef entitydef;
		animdef.m_frameRefs.push_back(3);
		animdef.m_frameRefs.push_back(4);
		animdef.m_frameRefs.push_back(5);
		animdef.m_frameDurations.push_back(sf::milliseconds(500));
		animdef.m_frameDurations.push_back(sf::milliseconds(500));
		animdef.m_frameDurations.push_back(sf::milliseconds(500));
		animdef.m_repeat = true;
		animdef.m_animType = GameAnimation::ANIM_TYPE_LINEAR;
		
		entitydef.m_geometryRef = 1;
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->CreateNewEntity(pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create new entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->CreateNewAnimation(pAnimation)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create new animation failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_spEntityFactory->CreateBasicEntity(entitydef, *m_pActualFlatWorldClient->GetPhysWorld(), *pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Set entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_spEntityFactory->CreateAnimation(animdef, *pAnimation)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Set animation failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->CreateNewAnimation(pAnimation2)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create new animation2 failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		pAnimation2->SetLogger(m_pLogger);
		pAnimation->SetLogger(m_pLogger);
		if(FWG_FAILED(result = pAnimation->GenerateStaticAnimation(*pAnimation2, sf::milliseconds(100))))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Generate static animation failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		pEntity->SetAnimation(pAnimation2);
		pEntity->SetBlendMode(sf::BlendAlpha);
		m_pActualFlatWorldClient->AddMoveableEntity(pEntity);
		
	}

	m_pActualFlatWorldClient->EnableDebugDraw();

	return result;
	
}


