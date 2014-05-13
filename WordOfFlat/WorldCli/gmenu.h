#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include <MyGUI/MyGUI_Gui.h>
#include <MyGUI/MyGUI_OgrePlatform.h>
#include <MyGUI/MyGUI_Widget.h>

#include <GameSystem/glog.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobject.h>

class ClientGameLogic;

// should be singleton??
class GameMenu : public RefObjectImpl<IRefObject> {
	GameLoggerPtr 		m_pLogger;
	MyGUI::Gui		   	*m_pGui;
	MyGUI::OgrePlatform *m_pGuiPlatform;
public:
	GameMenu();
	virtual ~GameMenu();
	
	GameErrorCode Initialize(GameLogger *pLogger, Ogre::RenderWindow *pWindow, Ogre::SceneManager *pSceneManager);
	
	GameErrorCode PrepareIngameMenu(ClientGameLogic *pGameLogic);

};

#endif // __GAME_MENU_H__
