#include "gclientengine.h"



static const wxChar* TESTING_TITLE = wxT("Flat World Client");

GameErrorCode GameClientEngine::CreateWindow()
{
	GameErrorCode result = FWG_NO_ERROR;
	// Create the main window
	m_pRoot = new Ogre::Root("", "");
	if (m_pRoot == NULL)
	{
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateWindow() : Create ogre root failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_pRoot->loadPlugin("RenderSystem_GL");
	Ogre::RenderSystemList::const_iterator it = m_pRoot->getAvailableRenderers().begin();
	Ogre::RenderSystem *pRenSys = NULL;
	
	if(m_pRoot->getAvailableRenderers().size() == 1)
	{
		pRenSys = *it;
	}
	m_pRoot->setRenderSystem(pRenSys);
	m_pRoot->initialise(false);
	
	
	m_pRenderWindow = m_pRoot->createRenderWindow(m_settings.m_screenTitle.ToStdString()
												, m_settings.m_screenWidth
												, m_settings.m_screenHeight
												, false
												, 0);
												
	if(m_pRenderWindow == NULL)
	{
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateWindow() : Create render window failed: 0x%08x"),
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

	//m_pSceneGenerator = new (std::nothrow) GameTestSceneGenerator();
	//if (m_pSceneGenerator == NULL) 
	//{
	//	return FWG_E_MEMORY_ALLOCATION_ERROR;
	//}
	
	//m_spEntityFactory.Attach(new (std::nothrow) GameEntityFactory());
	//if (m_spEntityFactory == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
	
	//if(FWG_FAILED(result = m_spEntityFactory->Initialize(m_spResHolder.In(), pLogger))) 
	//{
	//	FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::Initialize() : Initialize entity factory failed: 0x%08x"),
	//		m_pLogger, result, FWGLOG_ENDVAL);
	//	return result;
	//}
	
	m_isInitialized = true;
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

	while(true) {}
	return result;
}



GameErrorCode GameClientEngine::CreateTestingWorld()
{
	GameErrorCode result = FWG_NO_ERROR;

	return result;
	
}


