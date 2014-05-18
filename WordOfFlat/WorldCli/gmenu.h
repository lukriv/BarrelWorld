#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include "gmenumgr.h"

#include <GameSystem/glog.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobject.h>

class ClientGameLogic;

// game menu with buttons and so on
class GameMenu : public GameMenuBase {
	ClientGameLogic* m_pClientLogic;
	bool m_visible;
	
	// menu buttons
	MyGUI::Button* m_pButtonExit;
	MyGUI::Button* m_pButtonSwitch;
	
public:
	GameMenu(GameMenuResources *pMenuRes);
	virtual ~GameMenu();
	
	GameErrorCode Initialize(Ogre::RenderWindow* pWindow, Ogre::SceneManager* pSceneManager, ClientGameLogic *pCliLogic);
	
	GameErrorCode PrepareIngameMenu(ClientGameLogic *pGameLogic);
	
	void SwitchExitButton();
	
protected:
	virtual GameErrorCode Create();
	virtual GameErrorCode Destroy();

};

#endif // __GAME_MENU_H__
