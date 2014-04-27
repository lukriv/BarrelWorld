#include "gcompmgr.h"

GameCompManager::GameCompManager() {}

GameCompManager::~GameCompManager() {}

GameErrorCode GameCompManager::Initialize(Ogre::SceneManager* pSceneManager)
{
	FWG_RETURN_FAIL(m_renderMgr.Initialize(*pSceneManager));
	return FWG_NO_ERROR;
}

void GameCompManager::Uninitialize()
{
	m_entityMgr.DestroyAllEntities();
	m_renderMgr.Uninitialize();
}
