#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include <set>
#include <OGRE/OgreSceneManager.h>
#include <wx/thread.h>

#include <GameSystem/glog.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/gmap.h>
#include <GameResHold/gdeftables.h>
#include <MainWindow/gsettings.h>


// constants
static const wxChar* FWG_UNUSED(MAIN_CAMERA_NAME) = wxT("MainCamera");
static const wxChar* FWG_UNUSED(SECONDARY_CAMERA_NAME) = wxT("SecondCamera");

class RenderComponent;
class RenderObject;

class RenderCompManager
{
private:
	typedef GameBasMap< wxString, RefObjSmPtr<RenderObject> > TGameCameraMap;
	typedef wxVector< RenderComponent* > TUpdateQueue;
private:
	GameLoggerPtr m_spLogger;
	// ogre base
	Ogre::Root *m_pRoot;
	Ogre::RenderWindow *m_pRenderWindow;
	
	// optional scene manager - probably it will be only one in the project
	Ogre::SceneManager* m_pSceneManager;
	RefObjSmPtr<RenderObject> m_spMainCamera;
	TGameCameraMap m_cameraMap;
	wxCriticalSection m_mgrLock;
	wxCriticalSection m_processLock;
	
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
	
	inline Ogre::SceneManager* GetOgreSceneManager() { return m_pSceneManager; }
	
	inline Ogre::RenderWindow* GetOgreRenderWindow() { return m_pRenderWindow; }
	
	/*!
	 * \brief Get arbitrary camera by name
	 * \param cameraName
	 * \return 
	 */
	RenderObject* GetCamera(const wxString& cameraName);
	
	RenderObject* GetMainCamera() { return m_spMainCamera; }
	
	
	
	
	
	
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
	
	
	//////////////
	// Creators //
	//////////////
	
	/*!
	 * \brief Create empty render component
	 * \return Pointer to newly created render component (can be NULL)
	 */
	GameErrorCode CreateEmptyRenderComponent(RenderComponent *&pRenderComp);
	
	/**
	 * \brief Create render component from render component definition
	 * \param renderCompDef
	 * \param pRenderComp
	 */
	GameErrorCode CreateRenderComponent(const RenderDef &renderCompDef, RenderComponent *&pRenderComp);
	
	/*!
	 * \brief Create RenderObject from Render Object Definition
	 * 
	 * \param renderObjectDef
	 * \param pRenderObject
	 */
	GameErrorCode CreateRenderObject(const RenderEntityDef &renderObjectDef, RenderObject *&pRenderObject);
	
	/*!
	 * \brief Create ogre camera attached to its scene node
	 * \param pCamera
	 * \return Errorcode
	 */
	GameErrorCode CreateCamera(const CameraDef &cameraDef, RenderObject *&pCamera);	


};

#endif // __GAME_RENDER_COMPONENT_MANAGER_H__
