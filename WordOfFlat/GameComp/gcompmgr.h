#ifndef __GAME_COMPONENT_MANAGER_H__
#define __GAME_COMPONENT_MANAGER_H__

#include "OGRE/OgreSceneManager.h"
#include "gentitymgr.h"
#include "RenderComp/grendercmgr.h"

class GameCompManager : public RefObjectImpl<IRefObject> {
	RenderCompManager m_renderMgr;
	GameEntityManager m_entityMgr; // should be the last manager (first in destroy)
public:
	GameCompManager();
	~GameCompManager();
	
	GameErrorCode Initialize(Ogre::SceneManager *pSceneManager);
	void Uninitialize();
	
	RenderCompManager& GetRenderManager() { return m_renderMgr; }
	GameEntityManager& GetEntityManager() { return m_entityMgr; }

};

#endif // __GAME_COMPONENT_MANAGER_H__
