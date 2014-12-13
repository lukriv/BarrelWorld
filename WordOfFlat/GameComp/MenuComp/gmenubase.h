#ifndef __GAME_MENU_BASE_H__
#define __GAME_MENU_BASE_H__

class GameMenuSystem;

class GameMenuBase : public RefObjectImpl<IRefObject> {
protected:
	GameMenuSystem *m_pMenuRes;
	bool m_enabled;
	wxCriticalSection m_menuLock;
public:
	GameMenuBase();
	virtual ~GameMenuBase();
	
	/**
	 * \brief Shows this menu
	 * 
	 * All other menus will be disabled.
	 */
	GameErrorCode Enable();
	
	/**
	 * \brief Hides this menu
	 * 
	 * It disables this menu. Other menus does not change its state (will be hidden too).
	 * 
	 * @return 
	 */
	GameErrorCode Disable();
protected:
	void SetMenuSystem(GameMenuSystem *pMenuRes);

	virtual GameErrorCode Create() = 0;
	virtual void Destroy() = 0;
	
	friend class GameMenuSystem;
};


#endif //__GAME_MENU_BASE_H__