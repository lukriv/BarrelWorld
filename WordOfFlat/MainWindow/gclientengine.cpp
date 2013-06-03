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

GameClientEngine* GameClientEngine::GetEngine()
{
}

GameErrorCode GameClientEngine::Initialize(GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	if (m_isInitialized)
	{
		return FWG_NO_ERROR;
	}
	
	m_pLogger.Attach(pLogger);
	if(pLogger) {
		pLogger->addRef();
	}
	
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
		 

        // Clear screen
        window.clear();
 
        // Draw the sprite
        window.draw(sprite);
 
        // Draw the string
        //App.Draw(Text);
		
		// Update the window
        window.display();
    }
}



GameErrorCode GameClientEngine::CreateTestingWorld()
{
	GameErrorCode result = FWG_NO_ERROR;
	sf::Texture* pTexture = nullptr;
	sf::Shape* pShape = nullptr;
	
	m_pActualFlatWorldClient = new (std::nothrow) GameFlatWorldClient();
	if(m_pActualFlatWorldClient == nullptr)
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create flat world client failed: 0x%08x"),
			m_pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL)
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_pActualFlatWorldServer = new (std::nothrow) GameFlatWorldSrv();
	if(m_pActualFlatWorldServer == nullptr)
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create flat world server failed: 0x%08x"),
			m_pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL)
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if(FWG_FAILED(result = m_pActualFlatWorldClient->Initialize(pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Initialize flat world client failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = m_pActualFlatWorldServer->Initialize(pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Initialize flat world server failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	
	
}

GameErrorCode GameClientEngine::LoadTextures()
{
	 // Load a sprite to display
	 GameErrorCode result = FWG_NO_ERROR;
     wxScopedPtr<sf::Texture> apTexture = nullptr;
	 apTexture.reset(new (std::nothrow) sf::Texture());
	 
	 if (!apTexture->loadFromFile(loadFromFile("res/img/ground.png")))
	 {
		 FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::LoadTextures(): Loading resource file \"%s\" failed"),
				spLogger, wxT("res/img/ground.png"), FWGLOG_ENDVAL);
		 return FWG_E_MISC_ERROR;
	 }
	 
	 if(FWG_FAILED(result = m_pActualFlatWorldClient->AddTexture(1, apTexture.release())))
	 {
		 FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::LoadTextures() : Add texture 1 failed: 0x%08x"),
					spLogger, result, FWGLOG_ENDVAL);
		 return result;
	 }
	 
	 if (!apTexture->loadFromFile(loadFromFile("res/img/woodbox.png")))
	 {
		 FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::LoadTextures(): Loading resource file \"%s\" failed"),
				spLogger, wxT("res/img/woodbox.png"), FWGLOG_ENDVAL);
		 return FWG_E_MISC_ERROR;
	 }
	 
	 if(FWG_FAILED(result = m_pActualFlatWorldClient->AddTexture(2, apTexture.release())))
	 {
		 FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::LoadTextures() : Add texture 2 failed: 0x%08x"),
					spLogger, result, FWGLOG_ENDVAL);
		 return result;
	 }
	 
	 return FWG_NO_ERROR;
}

GameErrorCode GameClientEngine::LoadObjects()
{
	GameObjectSrvImpl *pSrvObj = nullptr;
	b2BodyDef bodyDef;
	bodyDef.position.Set(0.0f, -10.0f);
	m_pActualFlatWorldServer->CreateNewObject()
	
}

