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
	GameMenu(GameMenuSystem *pMenuRes, GameInputSystem *pInputSystem, GameLogger *pLogger);
	virtual ~GameMenu();
	
	GameErrorCode Initialize(GameMenuCallback*);
	
	// menu events
	bool ExitEvent(const CEGUI::EventArgs& /*e*/);
	bool SwitchEvent(const CEGUI::EventArgs& /*e*/);
	
	
	// input mouse callback virtual methods
	virtual void OnMouseMoved(const OIS::MouseState& arg) override;
	virtual void OnMousePressed(const OIS::MouseState& arg, OIS::MouseButtonID id) override;
	virtual void OnMouseReleased(const OIS::MouseState& arg, OIS::MouseButtonID id) override;
	
public:
	
	virtual GameErrorCode Show() override;
	
};

#endif // __GAME_MENU_H__
