#include "grenderobjmgr.h"
#include "grendersystem.h"




RenderObjectManager::RenderObjectManager(GameRenderSystem* pRenderSystem, GameEntityManager *pEntityMgr) : GameManagerBase(pRenderSystem->GetLogger(), pEntityMgr), m_pRenderSystem(pRenderSystem)
{
}

RenderObjectManager::~RenderObjectManager()
{
}
