#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include <GameSystem/glog.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobject.h>

#include "gmenumgr.h"
#include "gmenubase.h"

class GameMenuCallback {
public:
	
	virtual void OnExitEvent() = 0;
};


// game menu with buttons and so on
class GameMenu : public GameMenuBase {

	bool m_isInitialized;
	// menu buttons
	CEGUI::Window* m_pButtonExit;
	CEGUI::Window* m_pButtonSwitch;
	
	GameMenuCallback* m_pCallback;
	
public:
	GameMenu(GameMenuSystem *pMenuRes, GameLogger *pLogger);
	virtual ~GameMenu();
	
	GameErrorCode Initialize(GameMenuCallback*);
	
	// menu events
	bool ExitEvent(const CEGUI::EventArgs& /*e*/);
	bool SwitchEvent(const CEGUI::EventArgs& /*e*/);
	
public:
	
	virtual GameErrorCode Show() override;
	
};

#endif // __GAME_MENU_H__
