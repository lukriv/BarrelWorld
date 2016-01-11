#include "grenderobjmgr.h"
#include "grendersystem.h"




RenderObjectManager::RenderObjectManager(GameRenderSystem* pRenderSystem) : GameManagerBase(pRenderSystem->GetLogger()), m_pRenderSystem(pRenderSystem)
{
}

RenderObjectManager::~RenderObjectManager()
{
}
