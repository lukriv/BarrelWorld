#ifndef __GAME_MENU_MANAGER_H__
#define __GAME_MENU_MANAGER_H__

#include <set>
#include <MyGUI/MyGUI_Gui.h>
#include <MyGUI/MyGUI_OgrePlatform.h>
#include <MyGUI/MyGUI_Widget.h>

#include <GameSystem/glog.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobject.h>

class GameMenuBase;

// should be singleton??
class GameMenuSystem {
	typedef GameBasSet<GameMenuBase*> TMenuList;
private:
	GameLoggerPtr 		m_pLogger;
	MyGUI::Gui		   	*m_pGui;
	MyGUI::OgrePlatform *m_pGuiPlatform;
	TMenuList m_menuList;
	wxInt32 m_actualViewportIndex;
	wxCriticalSection m_systemLock;
public:
	GameMenuSystem(GameLogger *pLogger);
	virtual ~GameMenuSystem();
	
	GameErrorCode Initialize( Ogre::RenderWindow *pWindow, Ogre::SceneManager *pSceneManager);
	
	void Uninitialize();
	
	inline GameLogger* GetLogger() { return m_pLogger; }
	
	void SetActualViewportIndex(wxInt32 actViewportIndex);
	wxInt32 GetActualViewportIndex(); 
	
	MyGUI::Gui* GetGuiManager() { return m_pGui; }
	
	/**
	 * \brief Switches to requested menu
	 * 
	 * Switches to requested menu, all other menus are disabled.
	 * 
	 * \param pMenu Pointer to requested menu
	 */
	GameErrorCode SwitchMenu(GameMenuBase* pMenu);

private:
	void AddMenu (GameMenuBase* pMenu);
	void RemoveMenu (GameMenuBase* pMenu);

	friend class GameMenuBase;
};




#endif // __GAME_MENU_MANAGER_H__
