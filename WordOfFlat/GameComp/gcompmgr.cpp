#include "gcompmgr.h"

GameCompManager::GameCompManager(GameLogger *pLogger) : m_spLogger(pLogger)
														, m_entityMgr(pLogger)
														, m_renderMgr(pLogger)
{}

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
