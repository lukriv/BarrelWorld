#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include <set>
#include <OGRE/OgreSceneManager.h>
#include <wx/thread.h>

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include "gcamera.h"

// constants
static const wxChar* FWG_UNUSED(MAIN_CAMERA_NAME) = wxT("MainCamera");

class RenderComponent;

class RenderCompManager
{
private:
	GameLoggerPtr m_spLogger;
	Ogre::SceneManager* m_pSceneManager;
	RefObjSmPtr<GameCamera> m_spMainCamera;
	
public:
	RenderCompManager(GameLogger *pLogger);
	~RenderCompManager();
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	/*!
	 * \brief Initialize Render Component Manager
	 * 
	 * Also creates and prepare main camera.
	 * 
	 * \param sceneManager
	 * \return 
	 */
	GameErrorCode Initialize(Ogre::SceneManager& sceneManager);
	void Uninitialize();
	
	/*!
	 * \brief Create ogre camera attached to its scene node
	 * \param pCamera
	 * \return Errorcode
	 */
	GameErrorCode CreateCamera(const wxString& cameraName, GameCamera *&pCamera);
	
	GameCamera* GetMainCamera() { return m_spMainCamera; }
	
	/*!
	 * \brief Create render component
	 * \return Pointer to newly created render component (can be NULL)
	 */
	GameErrorCode CreateEmptyRenderComponent(RenderComponent *&pRenderComp);
	
	inline Ogre::SceneManager* GetOgreSceneManager() { return m_pSceneManager; }

};

#endif // __GAME_RENDER_COMPONENT_MANAGER_H__
