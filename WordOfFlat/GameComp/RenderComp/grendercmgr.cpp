#include "grendercmgr.h"

#include "grendercomp.h"
#include "../GameSystem/new.h"

RenderCompManager::RenderCompManager() :
	m_pSceneManager(NULL)
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
	DestroyAllRenderComponents();
	m_pSceneManager = NULL;
}

RenderCompManager::~RenderCompManager()
{
	Uninitialize();
}


GameErrorCode RenderCompManager::CreateEmptyRenderComponent(RenderComponent* pRenderCompOut)
{
	RenderComponent* pRenderComp = NULL;
	
	if(m_pSceneManager == NULL)
	{
		// render manager is not initialized
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	pRenderComp = new (std::nothrow) RenderComponent(this);
	if(pRenderComp != NULL)
	{
		// lock critical section
		wxCriticalSectionLocker lock(m_critSection);
		m_renderMemory.insert(pRenderComp);
	} else {
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	pRenderCompOut = pRenderComp;
	
	return FWG_NO_ERROR;
}

void RenderCompManager::DestroyRenderComponent(RenderComponent* pRenderComp)
{
	TRenderComponentSet::iterator iter;
	wxCriticalSectionLocker lock(m_critSection);
	
	iter = m_renderMemory.find(pRenderComp);
	if(iter != m_renderMemory.end())
	{
		m_renderMemory.erase(iter);
		delete pRenderComp; // delete item
	}
}

void RenderCompManager::DestroyAllRenderComponents()
{
	TRenderComponentSet::iterator iter;
	
	// Render component manager is not initialized -> return
	if(m_pSceneManager == NULL)
	{
		return;
	}
	
	for (iter = m_renderMemory.begin(); iter != m_renderMemory.end(); iter++)
	{
		delete *iter;
	}
	
	m_renderMemory.clear();
	
	m_pSceneManager->destroyAllMovableObjects();
}
