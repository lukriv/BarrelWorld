#include "grendercmgr.h"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreEntity.h>
#include <GameSystem/new.h>
#include <GameComp/gentity.h>
#include "grendercompbase.h"

static const char* MAIN_GAME_SCENE_MANAGER = "MainSceneManager";


RenderSystem::RenderSystem(GameLogger* pLogger) : m_spLogger(pLogger)
															, m_pRoot(nullptr)
															, m_pRenderWindow(nullptr)
															, m_pSceneManager(nullptr)
															, m_pMainCamera(nullptr)
															, m_actualQueue(0)
														
{}

GameErrorCode RenderSystem::Initialize(GameEngineSettings& settings)
{
	// is or is not initialized
	GameErrorCode result = FWG_NO_ERROR;
	try {
		// Create the main window
		m_pRoot = new Ogre::Root("", "");
		if (m_pRoot == nullptr) {
			result = FWG_E_MEMORY_ALLOCATION_ERROR;
			FWGLOG_ERROR_FORMAT(wxT("Create ogre root failed: 0x%08x"),
								m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}

		m_pRoot->loadPlugin("RenderSystem_GL");
		m_pRoot->loadPlugin("Plugin_OctreeSceneManager");
		Ogre::RenderSystemList::const_iterator it = m_pRoot->getAvailableRenderers().begin();
		Ogre::RenderSystem *pRenSys = nullptr;

		if(m_pRoot->getAvailableRenderers().size() == 1) {
			pRenSys = *it;
		}
		m_pRoot->setRenderSystem(pRenSys);
		m_pRoot->initialise(false);


		m_pRenderWindow = m_pRoot->createRenderWindow(settings.m_screenTitle.ToStdString()
						  , settings.m_screenWidth
						  , settings.m_screenHeight
						  , false
						  , 0);

		if(m_pRenderWindow == nullptr) {
			result = FWG_E_MEMORY_ALLOCATION_ERROR;
			FWGLOG_ERROR_FORMAT(wxT("Create render window failed: 0x%08x"),
								m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}

		// create scene manager for game logic
		//m_pSceneManager = m_pRoot->createSceneManager(Ogre::ST_GENERIC, Ogre::String(MAIN_GAME_SCENE_MANAGER));
		m_pSceneManager = m_pRoot->createSceneManager("OctreeSceneManager", Ogre::String(MAIN_GAME_SCENE_MANAGER));
		if(m_pSceneManager == nullptr) {
			result = FWG_E_MEMORY_ALLOCATION_ERROR;
			FWGLOG_ERROR_FORMAT(wxT("Create scene manager failed: 0x%08x"),
								m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		// initialize Ogre resources
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("res", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		
	} catch(Ogre::Exception &exc) {
		FWGLOG_ERROR_FORMAT(wxT("OGRE error message: '%s'"), m_spLogger
						, wxString::FromUTF8(exc.getFullDescription().c_str()).GetData().AsInternal()
						, FWGLOG_ENDVAL );
		return FWG_E_RENDER_SYSTEM_ERROR;
	}
	
	return result;
}

void RenderSystem::Uninitialize()
{
	if(m_pRenderWindow != nullptr) 
	{
		m_pRenderWindow->removeAllListeners();
		m_pRenderWindow->removeAllViewports();
		if (m_pRoot != nullptr) m_pRoot->detachRenderTarget(m_pRenderWindow);
		m_pRenderWindow->destroy();
		m_pRenderWindow = nullptr;
	}

	if(m_pSceneManager != nullptr)
	{
		m_pRoot->destroySceneManager(m_pSceneManager);
		m_pSceneManager = nullptr;
	}

	// delete ogre root
	if(m_pRoot != nullptr) 
	{
		delete m_pRoot;
		m_pRoot = nullptr;
	}
	
}

RenderSystem::~RenderSystem()
{
	Uninitialize();
}


void RenderSystem::StartRendering()
{
	DisableCreating();
	m_pRoot->startRendering();
}

void RenderSystem::DisableCreating()
{
	m_renderLock.Enter();
}

void RenderSystem::EnableCreating()
{
	m_renderLock.Leave();
}

Ogre::Camera* RenderSystem::GetCamera(const wxString& cameraName)
{
	try {
		return m_pSceneManager->getCamera(cameraName.ToStdString());
	} catch (Ogre::Exception &exc) {
		FWGLOG_ERROR_FORMAT(wxT("OGRE camera not found: '%s'"), m_spLogger
						, wxString::FromUTF8(exc.getFullDescription().c_str()).GetData().AsInternal()
						, FWGLOG_ENDVAL );
		return nullptr;
	}
	
}

GameErrorCode RenderSystem::AddToUpdateQueue(RenderComponentBase* pRenderComp)
{
	wxCriticalSectionLocker lock(m_mgrLock);
	m_updateQueue[m_actualQueue].push_back(pRenderComp);
	return FWG_NO_ERROR;
}

GameErrorCode RenderSystem::ProcessAllUpdates()
{
	TUpdateQueue *processQueue = nullptr;
	TUpdateQueue::iterator iter, endIter;
	
	wxCriticalSectionLocker processLock(m_processLock);
	{
		wxCriticalSectionLocker lock(m_mgrLock);
		processQueue = &m_updateQueue[m_actualQueue];
		m_actualQueue = (m_actualQueue + 1) % 2;
	}
	// disable render lock
	wxCriticalSectionLocker renderLock(m_renderLock);
	endIter = processQueue->end();
	for(iter = processQueue->begin(); iter != endIter; ++iter)
	{
		(*iter)->ProcessUpdate();
	}
	
	processQueue->clear();
	
	return FWG_NO_ERROR;
	
}

GameErrorCode RenderSystem::SetMainCamera(Ogre::Camera* pCamera)
{
	if (!pCamera)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	if(!m_pRenderWindow)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	wxCriticalSectionLocker renderLock(m_renderLock);
	wxCriticalSectionLocker lock(m_mgrLock);
	
	Ogre::Viewport *pViewport = nullptr;
	
	// check if main viewport exists
	if(m_pRenderWindow->hasViewportWithZOrder(0))
	{
		
		pViewport = m_pRenderWindow->getViewport(0);
		pViewport->setCamera(pCamera);
	} else {
		// create new main viewport
		pViewport = m_pRenderWindow->addViewport(pCamera);
	}
	
	pViewport->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));
	pCamera->setAspectRatio(Ogre::Real(pViewport->getActualWidth()) / Ogre::Real(pViewport->getActualHeight()));
	
	m_pMainCamera = pCamera;
	
	return FWG_NO_ERROR;	
}

GameErrorCode RenderSystem::SetMainCamera(const wxString& cameraName)
{
	Ogre::Camera* pCamera = nullptr;
	
	pCamera = GetCamera(cameraName);
	if(pCamera)
	{
		return SetMainCamera(pCamera);
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
}

GameErrorCode RenderSystem::ProcessAllCreation()
{
}

GameErrorCode RenderSystem::CreateRenderComponent(RenderComponentBase* pComponent, void* pContext)
{
}
