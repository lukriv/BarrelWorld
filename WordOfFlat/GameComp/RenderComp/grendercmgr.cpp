#include "grendercmgr.h"

#include <GameSystem/new.h>
#include "grendercomp.h"
#include "grenderobj.h"


RenderCompManager::RenderCompManager(GameLogger* pLogger) : m_spLogger(pLogger)
															, m_pSceneManager(nullptr)
														
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
	RenderComponent* pRenderComp = NULL;
	
	if(m_pSceneManager == NULL)
	{
		// render manager is not initialized
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	FWG_RETURN_FAIL(GameNewChecked(pRenderComp, this));
	
	pRenderCompOut = pRenderComp;
	
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
