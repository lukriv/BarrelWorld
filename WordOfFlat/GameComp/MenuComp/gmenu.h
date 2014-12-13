#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include "gmenumgr.h"

#include <GameSystem/glog.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobject.h>


class GameMenuCallback {
	
	void SetExitMenu(MyGUI::Widget* _sender) = 0;
	
	void SetShowExit(MyGUI::Widget* _sender) = 0;
	
};


// game menu with buttons and so on
class GameMenu : public GameMenuBase {

	// menu buttons
	MyGUI::Button* m_pButtonExit;
	MyGUI::Button* m_pButtonSwitch;
	
public:
	GameMenu();
	virtual ~GameMenu();
	
	GameErrorCode Initialize(GameMenuCallback*);
	
	void SwitchExitButton();
	
protected:
	virtual GameErrorCode Create() override;
	virtual void Destroy() override;

};

#endif // __GAME_MENU_H__
