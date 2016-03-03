#ifndef __GAME_MENU_MANAGER_H__
#define __GAME_MENU_MANAGER_H__

#include <set>
#include <CEGUI/CEGUI.h>
#include <OGRE/OgreRenderWindow.h>

#include <GameSystem/glog.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobject.h>

class GameMenuBase;

// should be singleton??
class GameMenuSystem {
private:
	GameLoggerPtr m_pLogger;
	wxCriticalSection m_systemLock;
	
	CEGUI::System *m_pGuiSystem;
public:
	GameMenuSystem(GameLogger *pLogger);
	virtual ~GameMenuSystem();
	
	GameErrorCode Initialize( Ogre::RenderWindow *pWindow );
	
	void Uninitialize();
	
	inline GameLogger* GetLogger() { return m_pLogger; }
	
	inline CEGUI::System *GetGuiSystem() { return m_pGuiSystem; }
	
	void GetPointerPosition(Ogre::Vector2 &output);
	
};




#endif // __GAME_MENU_MANAGER_H__
