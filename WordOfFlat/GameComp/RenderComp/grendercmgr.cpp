#include "grendercmgr.h"

#include <GameSystem/new.h>
#include "grendercomp.h"
#include "grenderobj.h"


RenderCompManager::RenderCompManager(GameLogger* pLogger) : m_spLogger(pLogger)
															, m_pSceneManager(nullptr)
															, m_actualQueue(0)
														
{}

GameErrorCode RenderCompManager::Initialize(Ogre::SceneManager& sceneManager)
{
	// is or is not initialized
	if(m_pSceneManager != NULL) 
	{
		return FWG_NO_ERROR;
	}
	
	m_pSceneManager = &sceneManager;
	
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
