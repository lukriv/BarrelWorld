#ifndef __GAME_CLIENT_ENGINE_H__
#define __GAME_CLIENT_ENGINE_H__


#include <GameSystem/glog.h>
#include <wx/string.h>
#include <OGRE/OgreRoot.h>
#include <OIS/OISInputManager.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/gcompmgr.h>
#include <GameResHold/gentityfactory.h>
#include "gsettings.h"


enum GameState {
	GAME_STATE_UNKNOWN 		= 0,
	GAME_STATE_EXIT			= 1,
	GAME_STATE_MAIN_MENU	= 2,
};


/*! \brief Engine for control game states and processes
 *
 * 	It is singleton and can be created only once.
 */
class GameClientEngine
{
private:
	GameLoggerPtr m_pLogger;
	GameEngineSettings m_settings;
	
	RefObjSmPtr<GameCompManager> m_spCompManager;
	
	RefObjSmPtr<GameEntityFactory> m_spFactory;

	bool m_isInitialized;
	bool m_exit;
private:

	GameErrorCode LoadSettings(wxChar* pFileName = nullptr);
	
public:
	GameClientEngine() : m_isInitialized(false),
		m_exit(false) {}

	/*! \brief Destructor */
	~GameClientEngine();

	/*! \brief Initialize game engine
	 *
	 * 	It loads settings, create window and initialize events.
	 * \return
	 */
	GameErrorCode Initialize(GameLogger* pLogger = nullptr);
	void Uninitialize();
	
	/**
	 * \brief Runs game engine main loop.
	 */
	GameErrorCode MainLoop();

	inline GameLogger* GetLogger() { return m_pLogger; }
	inline GameCompManager* GetCompManager() { return m_spCompManager; }
	inline GameEntityFactory* GetFactory() { return m_spFactory; }
	

// helper method
	GameErrorCode CreateTestingWorld();

};

#endif //__GAME_CLIENT_ENGINE_H__
