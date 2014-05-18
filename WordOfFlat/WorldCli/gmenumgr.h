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
class GameMenuResources : public RefObjectImpl<IRefObject> {
	typedef std::set<GameMenuBase*> TMenuList;
private:
	GameLoggerPtr 		m_pLogger;
	MyGUI::Gui		   	*m_pGui;
	MyGUI::OgrePlatform *m_pGuiPlatform;
	TMenuList m_menuList;
public:
	GameMenuResources();
	virtual ~GameMenuResources();
	
	GameErrorCode Initialize(GameLogger *pLogger, Ogre::RenderWindow *pWindow, Ogre::SceneManager *pSceneManager);
	
	MyGUI::Gui* GetGuiManager() { return m_pGui; }
	
	GameErrorCode SetGuiVisible(GameMenuBase* pMenu, wxInt32 actualVieportIndex);
	
	inline GameLogger* GetLogger() { return m_pLogger; }
	
	
	void AddMenu (GameMenuBase* pMenu);
	void RemoveMenu (GameMenuBase* pMenu);

};


class GameMenuBase : public RefObjectImpl<IRefObject> {
protected:
	RefObjSmPtr<GameMenuResources> m_spMenuRes;
public:
	GameMenuBase(GameMenuResources *pMenuRes);
	virtual ~GameMenuBase();
	
	GameErrorCode SetVisible(bool visibility, wxInt32 actualViewportIndex = 0);
protected:
	virtual GameErrorCode Create() = 0;
	virtual GameErrorCode Destroy() = 0;
	
	friend class GameMenuResources;
};

#endif // __GAME_MENU_MANAGER_H__
