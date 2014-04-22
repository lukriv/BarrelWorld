#ifndef __GAME_COMPONENT_MANAGER_H__
#define __GAME_COMPONENT_MANAGER_H__

#include "OGRE/OgreSceneManager.h"
#include "gentitymgr.h"
#include "RenderComp/grendercmgr.h"

class GameCompManager {
	GameEntityManager m_entityMgr;
	RenderCompManager m_renderMgr;
public:
	GameCompManager();
	~GameCompManager();
	
	GameErrorCode Initialize(Ogre::SceneManager *pSceneManager);
	void Uninitialize();
	
	RenderCompManager& GetRenderManager() { return m_renderMgr; }
	GameEntityManager& GetEntityManager() { return m_entityMgr; }

};

#endif // __GAME_COMPONENT_MANAGER_H__
