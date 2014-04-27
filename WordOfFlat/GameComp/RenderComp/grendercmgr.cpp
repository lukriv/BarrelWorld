#include "grendercmgr.h"

#include "grendercomp.h"
#include "../GameSystem/new.h"

RenderCompManager::RenderCompManager() : m_pSceneManager(NULL){}

GameErrorCode RenderCompManager::Initialize(Ogre::SceneManager& sceneManager)
{
	// is or is not initialized
	if(m_pSceneManager != NULL) 
	{
		return FWG_NO_ERROR;
	}
	
	m_pSceneManager = &sceneManager;
	
	// create new main camera
	FWG_RETURN_FAIL(CreateCamera(wxString(MAIN_CAMERA_NAME), m_spMainCamera.OutRef()));
	
	return FWG_NO_ERROR;
}

void RenderCompManager::Uninitialize()
{
	m_spMainCamera.Release();
	DestroyAllRenderComponents();
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

void RenderCompManager::DestroyAllRenderComponents()
{
	// Render component manager is not initialized -> return
	if(m_pSceneManager == NULL)
	{
		return;
	}
	
	m_pSceneManager->destroyAllMovableObjects();
}

GameErrorCode RenderCompManager::CreateCamera(const wxString& cameraName, GameCamera*& pGameCameraOut)
{
	GameErrorCode result = FWG_NO_ERROR;
	Ogre::Camera *pCamera = m_pSceneManager->createCamera(cameraName.ToStdString());
	Ogre::SceneNode *pSceneNode = m_pSceneManager->createSceneNode();
	GameCamera *pGameCamera = NULL;
	
	pSceneNode->attachObject(pCamera);
	
	if(FWG_FAILED(result = GameNewChecked(pGameCamera, pCamera)))
	{
		m_pSceneManager->destroyMovableObject(pCamera);
		m_pSceneManager->destroySceneNode(pSceneNode);
		return result;
	}
	
	pGameCameraOut = pGameCamera;
	
	return result;
}
