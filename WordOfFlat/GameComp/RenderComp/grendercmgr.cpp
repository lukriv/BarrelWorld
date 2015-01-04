#include "grendercmgr.h"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreEntity.h>
#include <GameSystem/new.h>
#include "grendercomp.h"
#include "grenderobj.h"

static const char* MAIN_GAME_SCENE_MANAGER = "MainSceneManager";


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
		                    m_spLogger, result, FWGLOG_ENDVAL);
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
		                    m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}

	// create scene manager for game logic
	m_pSceneManager = m_pRoot->createSceneManager(Ogre::ST_GENERIC, Ogre::String(MAIN_GAME_SCENE_MANAGER));
	if(m_pSceneManager == nullptr) {
		result = FWG_E_MEMORY_ALLOCATION_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("Create scene manager failed: 0x%08x"),
		                    m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// initialize Ogre resources
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("res", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, false);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	return FWG_NO_ERROR;
}

void RenderCompManager::Uninitialize()
{
	m_spMainCamera.Release();
	m_cameraMap.Clear();
	
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

RenderCompManager::~RenderCompManager()
{
	Uninitialize();
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
		processQueue = &m_updateQueue[m_actualQueue];
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

//////////////
// creators //
//////////////
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
	
	pRenderCompOut = spRenderComp.Detach();
	
	return FWG_NO_ERROR;
}

GameErrorCode RenderCompManager::CreateRenderComponent(const RenderDef& renderCompDef, RenderComponent*& pRenderComp)
{	
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<RenderComponent> spRenderComp;
	FWG_RETURN_FAIL(CreateEmptyRenderComponent(spRenderComp.OutRef()));
	
	if(!renderCompDef.m_entities.empty())
	{
		wxVector< RefObjSmPtr<RenderEntityDef> >::const_iterator iter;
		for(iter = renderCompDef.m_entities.begin(); iter != renderCompDef.m_entities.end(); ++iter)
		{
			RefObjSmPtr<RenderObject> spRenderObject;
			if(FWG_FAILED(result = CreateRenderObject(**iter, spRenderObject.OutRef())))
			{
				FWGLOG_ERROR_FORMAT(wxT("CreateRenderComponent failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			spRenderComp->AttachRenderObject(spRenderObject);
		}
	}
	
	if(!renderCompDef.m_cameras.empty())
	{
		wxVector< RefObjSmPtr<CameraDef> >::const_iterator iter;
		for(iter = renderCompDef.m_cameras.begin(); iter != renderCompDef.m_cameras.end(); iter++)
		{
			RefObjSmPtr<RenderObject> spRenderObject;
			if(FWG_FAILED(result = CreateCamera(**iter, spRenderObject.OutRef())))
			{
				FWGLOG_ERROR_FORMAT(wxT("CreateCamera failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			spRenderComp->AttachRenderObject(spRenderObject);
		}
	}
	
	pRenderComp = spRenderComp.Detach();
	
	return result;
}

GameErrorCode RenderCompManager::CreateRenderObject(const RenderEntityDef& renderObjectDef, RenderObject*& pRenderObject)
{
	// render entity is defined
	Ogre::Entity *pEntity = nullptr;
	RefObjSmPtr<RenderObject> spRenderObject;
	
	
	if(!renderObjectDef.GetName().IsEmpty())
	{
		pEntity = m_pSceneManager->createEntity(renderObjectDef.GetName().ToStdString(), renderObjectDef.m_mesh->m_name.ToStdString());
	} else {
		FWGLOG_ERROR(wxT("Render entity name is empty"), m_spLogger);
		return FWG_E_NOT_IMPLEMENTED_ERROR;
	}
	
	//pEntity->setMaterialName(entityDef.m_material->m_name.ToStdString());
	pEntity->setMaterialName(renderObjectDef.m_material->m_name.ToStdString());
	
	
	FWG_RETURN_FAIL(GameNewChecked(spRenderObject.OutRef(), pEntity));
	
	pRenderObject = spRenderObject.Detach();
	
	return FWG_NO_ERROR;
}

GameErrorCode RenderCompManager::CreateCamera(const CameraDef &cameraDef, RenderObject*& pGameCameraOut)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<RenderObject> spCameraObject;
	Ogre::Camera *pOgreCam = nullptr;
	
	// create ogre camera
	pOgreCam = m_pSceneManager->createCamera(cameraDef.GetName().ToStdString());
	if(pOgreCam == nullptr)
	{
		FWGLOG_ERROR_FORMAT(wxT("Get Ogre camera failed: 0x%08x"), m_spLogger, FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}

	// Position it at 500 in Z direction
	//pOgreCam->setPosition(Ogre::Vector3(0, 10, -5));
	pOgreCam->setPosition(cameraDef.m_position);
	// Look back along -Z
	pOgreCam->setDirection(cameraDef.m_direction);
	pOgreCam->setNearClipDistance(cameraDef.m_near);
	
	if(FWG_FAILED(result = GameNewChecked(spCameraObject.OutRef(), pOgreCam)))
	{
		m_pSceneManager->destroyMovableObject(pOgreCam);
		return result;
	}
	
	if(FWG_FAILED(result = m_cameraMap.Insert(cameraDef.GetName().ToStdString(), spCameraObject)))
	{
		return result;
	}
		
	pGameCameraOut = spCameraObject.Detach();
		
	return result;
}

GameErrorCode RenderCompManager::SetMainCamera(RenderObject* pCameraObject)
{
	if ((!pCameraObject)||(!pCameraObject->IsCameraType()))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	if(!m_pRenderWindow)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	wxCriticalSectionLocker lock(m_mgrLock);
	
	Ogre::Viewport *pViewport = nullptr;
	Ogre::Camera *pCamera = pCameraObject->GetCamera();
	
	// check if main viewport exists
	if(m_pRenderWindow->hasViewportWithZOrder(0))
	{
		
		pViewport = m_pRenderWindow->getViewport(0);
		pViewport->setCamera(pCamera);
	} else {
		// create new main viewport
		pViewport = m_pRenderWindow->addViewport(pCameraObject->GetCamera());
	}
	
	pViewport->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));
	pCamera->setAspectRatio(Ogre::Real(pViewport->getActualWidth()) / Ogre::Real(pViewport->getActualHeight()));
	
	m_spMainCamera = pCameraObject;
	
	return FWG_NO_ERROR;	
}

GameErrorCode RenderCompManager::SetMainCamera(const wxString& cameraName)
{
	RefObjSmPtr<RenderObject> spCamera;
	
	spCamera = GetCamera(cameraName);
	if(!spCamera.IsEmpty())
	{
		return SetMainCamera(spCamera);
	} else {
		return FWG_E_OBJECT_NOT_FOUND_ERROR;
	}
}
