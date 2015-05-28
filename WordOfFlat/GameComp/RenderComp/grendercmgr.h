#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include <OGRE/OgreSceneManager.h>
#include <wx/thread.h>

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/gmap.h>
#include <MainWindow/gsettings.h>


// constants
static const wxChar* FWG_UNUSED(MAIN_CAMERA_NAME) = wxT("MainCamera");
static const wxChar* FWG_UNUSED(SECONDARY_CAMERA_NAME) = wxT("SecondCamera");

class RenderComponent;
class RenderCamera;
class GameEntity;

class RenderCompManager
{
private:
	typedef wxVector< RenderComponent* > TUpdateQueue;
private:
	GameLoggerPtr m_spLogger;
	// ogre base
	Ogre::Root *m_pRoot;
	Ogre::RenderWindow *m_pRenderWindow;
	
	// optional scene manager - probably it will be only one in the project
	Ogre::SceneManager* m_pSceneManager;
	Ogre::Camera* m_pMainCamera;
	wxCriticalSection m_mgrLock;
	wxCriticalSection m_processLock;
	wxCriticalSection m_renderLock;
	
	TUpdateQueue m_updateQueue[2];
	wxDword m_actualQueue;
	
public:
	RenderCompManager(GameLogger *pLogger);
	~RenderCompManager();
	
	/*!
	 * \brief Initialize Render Component Manager
	 * 
	 * Also creates and prepare main camera.
	 * 
	 * \param sceneManager
	 * \return 
	 */
	GameErrorCode Initialize(GameEngineSettings& settings);
	void Uninitialize();
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	inline Ogre::Root* GetOgreRoot() { return m_pRoot; }
	
	inline Ogre::SceneManager* GetOgreSceneManager() { return m_pSceneManager; }
	
	inline Ogre::RenderWindow* GetOgreRenderWindow() { return m_pRenderWindow; }
	
	/*!
	 * \brief Get arbitrary camera by name
	 * \param cameraName
	 * \return 
	 */
	Ogre::Camera* GetCamera(const wxString& cameraName);
	
	Ogre::Camera* GetMainCamera() { return m_pMainCamera; }
	
	GameErrorCode SetMainCamera( Ogre::Camera *pCameraObject);
	
	GameErrorCode SetMainCamera( const wxString& cameraName );
	
	/**
	 * @brief Starts rendering
	 * 
	 * It calls Ogre::Root->startRendering() and after this call is blocked creating render component
	 * until the EnableCreating() is called.
	 * 
	 */
	void StartRendering();
	
	void EnableCreating();
	
	void DisableCreating();
	
	/**
	 * \brief Add component to update queue
	 * \param pRenderComp Pointer to render component
	 */
	GameErrorCode AddToUpdateQueue(RenderComponent *pRenderComp);
	
	/**
	 * \brief Updates all components in update queue
	 * @return FWG_NO_ERROR on success.
	 */
	GameErrorCode ProcessAllUpdates();
	
	
};

#endif // __GAME_RENDER_COMPONENT_MANAGER_H__
