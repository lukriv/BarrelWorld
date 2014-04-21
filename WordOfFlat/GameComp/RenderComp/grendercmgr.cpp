#include "grendercmgr.h"

#include "grendercomp.h"

RenderCompManager::RenderCompManager(Ogre::SceneManager* pSceneManager) :
	m_pSceneManager(pSceneManager)
{}


RenderCompManager::~RenderCompManager()
{
}


RenderComponent* RenderCompManager::CreateEmptyRenderComponent()
{
	RenderComponent* pRenderComp = NULL;
	
	pRenderComp = new (std::nothrow) RenderComponent();
}
