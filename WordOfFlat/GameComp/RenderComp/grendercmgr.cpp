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


RenderComponent* RenderCompManager::CreateEmptyRenderComponent()
{
	RenderComponent* pRenderComp = NULL;
	
	GameNew(pRenderComp, this);
	if(pRenderComp != NULL)
	{
		// lock critical section
		wxCriticalSectionLocker lock(m_critSection);
		m_renderMemory.insert(pRenderComp);
	}
	
	return pRenderComp;
}

void RenderCompManager::DestroyRenderComponent(RenderComponent* pRenderComp)
{
	TRenderComponentSet::iterator iter;
	wxCriticalSectionLocker lock(m_critSection);
	
	iter = m_renderMemory.find(pRenderComp);
	if(iter != m_renderMemory.end())
	{
		if(pRenderComp->GetType() != RENDER_COMP_UNDEFINED)
		{
			m_pSceneManager->destroyMovableObject(pRenderComp->GetOgreObject());
		}
		m_renderMemory.erase(iter);
	}
}

void RenderCompManager::DestroyAllRenderComponents()
{
	TRenderComponentSet::iterator iter;
	for (iter = m_renderMemory.begin(); iter != m_renderMemory.end(); iter++)
	{
		delete *iter;
	}
	
	m_renderMemory.clear();
	
	m_pSceneManager->destroyAllMovableObjects();
}
