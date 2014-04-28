#ifndef __GAME_CLIENT_ENGINE_H__
#define __GAME_CLIENT_ENGINE_H__


#include "../GameSystem/glog.h"
#include <wx/string.h>
#include <OGRE/OgreRoot.h>
#include <OIS/OISInputManager.h>
#include "../GameSystem/refobjectsmptr.h"
#include "../GameComp/gcompmgr.h"
#include "../GameResHold/gdefholder.h"
#include "../GameResHold/gentityfactory.h"
#include "ginputsystem.h"
#include "MyGUI/MyGUI_Gui.h"
#include "MyGUI/MyGUI_OgrePlatform.h"
#include "MyGUI/MyGUI_Widget.h"

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
	
	MyGUI::Gui		   *m_pGui;
	MyGUI::OgrePlatform *m_pGuiPlatform;
	
	OIS::InputManager*	m_pInputMgr;

	GameInputSystem* m_pInputSystem;
	
	GameCompManager m_componentManager;
	RefObjSmPtr<GameDefinitionHolder> m_spDefHolder;
	RefObjSmPtr<GameEntityFactory> m_spEntityFactory;
	
	bool m_isWindowCreated;
	bool m_isSettingLoaded;
	bool m_isInitialized;
	bool m_exit;
private:
	GameErrorCode CreateWindow();
	GameErrorCode LoadSettings(wxChar* pFileName = NULL);

public:
	GameClientEngine() : m_pRoot(NULL),
					m_pRenderWindow(NULL),
					m_pGui(NULL),
					m_pGuiPlatform(NULL),
					m_pInputMgr(NULL),
					m_pInputSystem(NULL),
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
	
	void SetExitInputClbk(bool exit) { m_exit = true; }
	void SetExit(MyGUI::Widget* _sender) { m_exit = true; }

};

#endif //__GAME_CLIENT_ENGINE_H__
