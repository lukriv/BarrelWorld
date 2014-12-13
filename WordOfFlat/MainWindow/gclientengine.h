#ifndef __GAME_CLIENT_ENGINE_H__
#define __GAME_CLIENT_ENGINE_H__


#include <GameSystem/glog.h>
#include <wx/string.h>
#include <OGRE/OgreRoot.h>
#include <OIS/OISInputManager.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/gcompmgr.h>
#include <GameResHold/gdefholder.h>
#include <GameResHold/gentityfactory.h>
#include <WorldCli/gmenu.h>
#include <WorldCli/gclientlogic.h>
#include "ginputsystem.h"
#include "gsettings.h"


/*! \brief Engine for control game states and processes
 *
 * 	It is singleton and can be created only once.
 */
class GameClientEngine
{
private:
	struct GameRenderListener : public Ogre::FrameListener {
		GameClientEngine *m_pOwner;
	public:
		GameRenderListener(GameClientEngine *pOwner) : m_pOwner(pOwner) {}
		// Ogre::FrameListener event
		
		virtual bool frameStarted (	const Ogre::FrameEvent & evt );	

		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		
		virtual bool frameEnded	( const Ogre::FrameEvent & evt );

	};
private:
	GameLoggerPtr m_pLogger;
	GameEngineSettings m_settings;
	
	RefObjSmPtr<GameCompManager> m_spCompManager;
	

	

	RefObjSmPtr<GameMenuSystem> m_spGameMenu;
	RefObjSmPtr<ClientGameLogic> m_spGameLogic;
	RefObjSmPtr<GameDefinitionHolder> m_spDefHolder;

	bool m_isWindowCreated;
	bool m_isSettingLoaded;
	bool m_isInitialized;
	bool m_exit;
private:
	GameErrorCode CreateWindow();
	GameErrorCode LoadSettings(wxChar* pFileName = NULL);
	GameErrorCode InitializeCameras();
	GameErrorCode InitializeMenus();
	GameErrorCode InitializeLights();
	GameErrorCode InitializeInputs();

public:
	GameClientEngine() : m_pRoot(NULL),
		m_pRenderWindow(NULL),
		m_pInputMgr(NULL),
		m_isWindowCreated(false),
		m_isSettingLoaded(false),
		m_isInitialized(false),
		m_exit(false) {}

	/*! \brief Destructor */
	~GameClientEngine();

	/*! \brief Initialize game engine
	 *
	 * 	It loads settings, create window and initialize events.
	 * \return
	 */
	GameErrorCode Initialize(GameLogger* pLogger = NULL);
	GameErrorCode MainLoop();

	GameErrorCode CreateTestingWorld();

	void SetExitInputClbk(bool exit) {
		m_exit = true;
	}
	void SetExit(MyGUI::Widget* _sender) {
		m_exit = true;
	}
	


};

#endif //__GAME_CLIENT_ENGINE_H__
