#ifndef __GAME_MENU_BASE_H__
#define __GAME_MENU_BASE_H__


#include <wx/thread.h>
#include <CEGUI/Window.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/glog.h>


class GameMenuSystem;

class GameMenuBase : public RefObjectImpl<IRefObject> {
private:
	CEGUI::Window *m_pRootWindow;
protected:
	GameLoggerPtr m_spLogger;
	GameMenuSystem *m_pMenuRes;
	
	wxCriticalSection m_menuLock;
public:
	GameMenuBase(GameMenuSystem *pMenuRes, GameLogger* pLogger);
	virtual ~GameMenuBase();
	
	/**
	 * \brief Shows this menu
	 * 
	 * All other menus will be hidden.
	 */
	virtual GameErrorCode Show() = 0;

protected:
	/**
	 * \brief Get menu root window 
	 */
	inline CEGUI::Window* GetRootWindow() { return m_pRootWindow; }
	
	GameErrorCode CreateRootWindow( const wxString &windowName );
	
	GameLoggerPtr GetLogger() { return m_spLogger; }
	
	friend class GameMenuSystem;
};


#endif //__GAME_MENU_BASE_H__