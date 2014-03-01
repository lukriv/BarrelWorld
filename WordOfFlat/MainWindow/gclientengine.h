#ifndef __GAME_CLIENT_ENGINE_H__
#define __GAME_CLIENT_ENGINE_H__


#include "../GameSystem/glog.h"
#include <wx/string.h>
#include <OGRE/OgreRoot.h>
#include "../GameSystem/refobjectsmptr.h"

struct GameEngineSettings {
	wxDword m_screenWidth;
	wxDword m_screenHeight;
	wxString m_screenTitle;
	wxString m_worldName;
	
	GameEngineSettings() : 
		m_screenWidth(800),
		m_screenHeight(600) {}
};

/*! \brief Engine for control game states and processes
 * 
 * 	It is singleton and can be created only once.
 */
class GameClientEngine {
private:
	GameLoggerPtr m_pLogger;
	GameEngineSettings m_settings;
	Ogre::Root *m_pRoot;
	Ogre::RenderWindow *m_pRenderWindow;
	bool m_isWindowCreated;
	bool m_isSettingLoaded;
	bool m_isInitialized;
private:
	GameErrorCode CreateWindow();
	GameErrorCode LoadSettings(wxChar* pFileName = NULL);

public:
	GameClientEngine() : m_pRoot(NULL),
					m_pRenderWindow(NULL),
					m_isWindowCreated(false),
					m_isSettingLoaded(false),
					m_isInitialized(false) {}

	/*! \brief Destructor */
	~GameClientEngine() {}

	/*! \brief Initialize game engine
	 * 
	 * 	It loads settings, create window and initialize events.
	 * \return 
	 */
	GameErrorCode Initialize(GameLogger* pLogger = NULL);
	GameErrorCode MainLoop();
	
	GameErrorCode CreateTestingWorld();

};

#endif //__GAME_CLIENT_ENGINE_H__
