#ifndef __GAME_MENU_BASE_H__
#define __GAME_MENU_BASE_H__


#include <wx/thread.h>
#include <CEGUI/Window.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/glog.h>

#include <GameComp/InputComp/ginputsystem.h>

class GameMenuSystem;

class GameMenuBase : public RefObjectImpl<IRefObject>, public GameInputSystem::InputMouseCallback{
private:
	CEGUI::Window *m_pRootWindow;
protected:
	GameLoggerPtr m_spLogger;
	GameMenuSystem *m_pMenuRes;
	GameInputSystem *m_pInputSystem;
	
	wxCriticalSection m_menuLock;
public:
	GameMenuBase(GameMenuSystem *pMenuRes, GameInputSystem *pInputSystem, GameLogger* pLogger);
	virtual ~GameMenuBase();
	
	/**
	 * \brief Shows this menu
	 * 
	 * All other menus will be hidden.
	 */
	virtual GameErrorCode Show() = 0;
	
	
	// input mouse callback virtual methods
	virtual void OnMouseMoved(const OIS::MouseState& arg) override;
	virtual void OnMousePressed(const OIS::MouseState& arg, OIS::MouseButtonID id) override;
	virtual void OnMouseReleased(const OIS::MouseState& arg, OIS::MouseButtonID id) override;

protected:
	/**
	 * \brief Get menu root window 
	 */
	inline CEGUI::Window* GetRootWindow() { return m_pRootWindow; }
	
	GameErrorCode CreateRootWindow( const wxString &windowName );
	
	GameLoggerPtr GetLogger() { return m_spLogger; }
	
	friend class GameMenuSystem;
};

// menu utilities
CEGUI::MouseButton ConvertMouseButtonId(OIS::MouseButtonID mouseButtonId);


#endif //__GAME_MENU_BASE_H__