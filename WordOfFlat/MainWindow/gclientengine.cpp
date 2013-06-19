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
	
	
	if(FWG_FAILED(result = m_pActualFlatWorldClient->Initialize(pLogger)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Initialize flat world client failed: 0x%08x"),
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
	GameErrorCode result = FWG_NO_ERROR;
		
	
	wxScopedPtr<GameEntity> apEntity;

	apEntity.reset(new (std::nothrow) GameEntity(GAME_OBJECT_TYPE_DYNAMIC_ENTITY));
	if (apEntity.get() == nullptr) return FWG_E_MEMORY_ALLOCATION_ERROR;
	
	apEntity->SetTexture(m_pActualFlatWorldClient->GetTexture(1));
	apEntity->SetGeometry(m_pActualFlatWorldClient->GetGeometry(1));
	apEntity->setPosition(0.0f, 5.0f);
	
	m_pActualFlatWorldClient->AddEntity(1, apEntity.release());


	apEntity.reset(new (std::nothrow) GameEntity(GAME_OBJECT_TYPE_STATIC_ENTITY));
	if (apEntity.get() == nullptr) return FWG_E_MEMORY_ALLOCATION_ERROR;
	
	apEntity->SetTexture(m_pActualFlatWorldClient->GetTexture(2));
	apEntity->SetGeometry(m_pActualFlatWorldClient->GetGeometry(2));
	apEntity->setPosition(0.0f, -10.0f);
	
	m_pActualFlatWorldClient->AddEntity(2, apEntity.release());	
	
	return result;	
	
}

GameErrorCode GameClientEngine::CreateEngine(GameClientEngine*& pEngine, GameLogger* pLogger)
{
	GameErrorCode result = FWG_NO_ERROR;
	if(!m_pClientEngine) 
	{
		m_pClientEngine = new (std::nothrow) GameClientEngine();
		if (m_pClientEngine == nullptr)
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateEngine() : Memory allocation error: 0x%08x"),
						pLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
		
		if (FWG_FAILED(result = m_pClientEngine->Initialize(pLogger)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateEngine() : Engine initialization failed: 0x%08x"),
						pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	}
	
	pEngine = m_pClientEngine;
	return FWG_NO_ERROR;
}

GameClientEngine* GameClientEngine::GetEngine()
{
	return m_pClientEngine;
}

GameErrorCode GameClientEngine::LoadShapes()
{
	wxScopedPtr<sf::VertexArray> apVertexArray;
	wxScopedPtr<GameSFMLGeometry> apGeometry;
	
	apVertexArray.reset(new (std::nothrow) sf::VertexArray(sf::Quads, 4));
	(*apVertexArray)[0].position = sf::Vector2f(-0.5f, -0.5f);
	(*apVertexArray)[1].position = sf::Vector2f(0.5f, -0.5f);
	(*apVertexArray)[2].position = sf::Vector2f(0.5f, 0.5f);
	(*apVertexArray)[3].position = sf::Vector2f(-0.5f, 0.5f);
	
	(*apVertexArray)[0].texCoords = sf::Vector2f(0, 0);
	(*apVertexArray)[1].texCoords = sf::Vector2f(256, 0);
	(*apVertexArray)[2].texCoords = sf::Vector2f(256, 256);
	(*apVertexArray)[3].texCoords = sf::Vector2f(0, 256);
	
	apGeometry = new (std::nothrow) GameSFMLGeometry(apVertexArray.release());
	
	m_pActualFlatWorldClient.AddGeometry(1, apGeometry.release());
	
	apVertexArray.reset(new (std::nothrow) sf::VertexArray(sf::Quads, 4));
	(*apVertexArray)[0].position = sf::Vector2f(-10, -0.5f);
	(*apVertexArray)[1].position = sf::Vector2f(10, -0.5f);
	(*apVertexArray)[2].position = sf::Vector2f(10, 0.5f);
	(*apVertexArray)[3].position = sf::Vector2f(-10, 0.5f);
	
	(*apVertexArray)[0].texCoords = sf::Vector2f(0, 0);
	(*apVertexArray)[1].texCoords = sf::Vector2f(256, 0);
	(*apVertexArray)[2].texCoords = sf::Vector2f(256, 256);
	(*apVertexArray)[3].texCoords = sf::Vector2f(0, 256);
	
	apGeometry = new (std::nothrow) GameSFMLGeometry(apVertexArray.release());
	
	m_pActualFlatWorldClient.AddGeometry(2, apGeometry.release());
	
	return FWG_NO_ERROR;
	
}
