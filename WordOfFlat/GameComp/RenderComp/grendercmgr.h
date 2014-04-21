#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include <set>
#include <OGRE/OgreSceneManager.h>

class RenderComponent;

class RenderCompManager
{
private:
	typedef std::set<RenderComponent*> TRenderComponentSet;
private:
	Ogre::SceneManager* m_pSceneManager;
	TRenderComponentSet m_renderMemory;
	
public:
	RenderCompManager(Ogre::SceneManager& sceneManager);
	~RenderCompManager();
	
	
	/*!
	 * \brief Create render component
	 * \return Pointer to newly created render component (can be NULL)
	 */
	RenderComponent* CreateEmptyRenderComponent();
	
	
	inline Ogre::SceneManager* GetOgreSceneManager() { return m_pSceneManager; }

};

#endif // __GAME_RENDER_COMPONENT_MANAGER_H__
