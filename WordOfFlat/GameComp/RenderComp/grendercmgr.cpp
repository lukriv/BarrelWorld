#include "grendercmgr.h"

#include <GameSystem/new.h>
#include "grendercomp.h"
#include "grenderobj.h"


RenderCompManager::RenderCompManager(GameLogger* pLogger) : m_spLogger(pLogger)
															, m_pRoot(nullptr)
															, m_pRenderWindow(nullptr)
															, m_pSceneManager(nullptr)
															, m_actualQueue(0)
														
{}

GameErrorCode RenderCompManager::Initialize(GameEngineSettings& settings)
{
	// is or is not initialized
	GameErrorCode result = FWG_NO_ERROR;
	
	// Create the main window
	m_pRoot = new Ogre::Root("", "");
	if (m_pRoot == nullptr) {
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("Create ogre root failed: 0x%08x"),
		                    m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	m_pRoot->loadPlugin("RenderSystem_GL");
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
		                    m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	// create scene manager for game logic
	m_pSceneManager = m_pRoot->createSceneManager(Ogre::ST_GENERIC, Ogre::String(MAIN_GAME_SCENE_MANAGER));
	if(m_pSceneManager == nullptr) {
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("Create scene manager failed: 0x%08x"),
		                    m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}

void RenderCompManager::Uninitialize()
{
	m_spMainCamera.Release();
	m_cameraMap.Clear();
	m_pSceneManager = NULL;
}

RenderCompManager::~RenderCompManager()
{
	Uninitialize();
}


GameErrorCode RenderCompManager::CreateEmptyRenderComponent(RenderComponent *&pRenderCompOut)
{
	RefObjSmPtr<RenderComponent> spRenderComp;
	
	if(m_pSceneManager == nullptr)
	{
		// render manager is not initialized
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	FWG_RETURN_FAIL(GameNewChecked(spRenderComp.OutRef(), this));
	
	FWG_RETURN_FAIL(spRenderComp->Initialize());
	
	pRenderCompOut = spRenderComp;
	
	return FWG_NO_ERROR;
}


GameErrorCode RenderCompManager::CreateCamera(const wxString& cameraName, RenderObject*& pGameCameraOut)
{
	GameErrorCode result = FWG_NO_ERROR;
	Ogre::Camera *pCamera = m_pSceneManager->createCamera(cameraName.ToStdString());
	RenderObject *pGameCamera = NULL;
	
	if(FWG_FAILED(result = GameNewChecked(pGameCamera, pCamera)))
	{
		m_pSceneManager->destroyMovableObject(pCamera);
		return result;
	}
	
	if(FWG_FAILED(result = m_cameraMap.Insert(cameraName, pGameCamera)))
	{
		return result;
	}
		
	pGameCameraOut = pGameCamera;
		
	return result;
}

RenderObject* RenderCompManager::GetCamera(const wxString& cameraName)
{
	TGameCameraMap::Iterator iter;
	
	iter = m_cameraMap.Find(cameraName);
	if(iter != m_cameraMap.End())
	{
		return iter->second.In();
	}
	
	return nullptr;
	
}

GameErrorCode RenderCompManager::AddToUpdateQueue(RenderComponent* pRenderComp)
{
	wxCriticalSectionLocker lock(m_mgrLock);
	m_updateQueue[m_actualQueue].push_back(pRenderComp);
	return FWG_NO_ERROR;
}

GameErrorCode RenderCompManager::ProcessAllUpdates()
{
	TUpdateQueue *processQueue = nullptr;
	TUpdateQueue::iterator iter, endIter;
	
	wxCriticalSectionLocker processLock(m_processLock);
	{
		wxCriticalSectionLocker lock(m_mgrLock);
		processQueue = m_updateQueue[m_actualQueue];
		m_actualQueue = (m_actualQueue + 1) % 2;
	}
	endIter = processQueue->end();
	for(iter = processQueue->begin(); iter != endIter; ++iter)
	{
		(*iter)->ProcessUpdate();
	}
	
	processQueue->clear();
	
	return FWG_NO_ERROR;
	
}
