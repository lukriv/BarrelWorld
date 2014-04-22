#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include <set>
#include <OGRE/OgreSceneManager.h>
#include <wx/thread.h>

#include "../../GameSystem/gerror.h"

class RenderComponent;

class RenderCompManager
{
private:
	typedef std::set<RenderComponent*> TRenderComponentSet;
private:
	Ogre::SceneManager* m_pSceneManager;
	TRenderComponentSet m_renderMemory;
	wxCriticalSection 	m_critSection;
	
public:
	RenderCompManager();
	~RenderCompManager();
	
	GameErrorCode Initialize(Ogre::SceneManager& sceneManager);
	void Uninitialize();
	
	
	
	/*!
	 * \brief Create render component
	 * \return Pointer to newly created render component (can be NULL)
	 */
	RenderComponent* CreateEmptyRenderComponent();
	
	void DestroyRenderComponent(RenderComponent* pRenderComp);
	
	void DestroyAllRenderComponents();
	
	inline Ogre::SceneManager* GetOgreSceneManager() { return m_pSceneManager; }

};

#endif // __GAME_RENDER_COMPONENT_MANAGER_H__
