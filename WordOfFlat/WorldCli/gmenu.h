#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include "MyGUI/MyGUI_Gui.h"
#include "MyGUI/MyGUI_OgrePlatform.h"
#include "MyGUI/MyGUI_Widget.h"


// should be singleton??
class GameMenu : public RefObjectImpl<IRefObject> {
	GameLoggerPtr 		m_pLogger;
	MyGUI::Gui		   	*m_pGui;
	MyGUI::OgrePlatform *m_pGuiPlatform;
public:
	GameMenu();
	virtual ~GameMenu();
	
	GameErrorCode Initialize(GameLogger *pLogger, Ogre::RenderWindow *pWindow, Ogre::SceneManager *pSceneManager);

};

#endif // __GAME_MENU_H__
