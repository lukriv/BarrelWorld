#ifndef __GAME_MAIN_MENU_STATE_H__
#define __GAME_MAIN_MENU_STATE_H__

#include <OGRE/OgreFrameListener.h>
#include <GameComp/MenuComp/gmenu.h>
#include "gclientengine.h"

class CDebugDraw;

/**
 * \brief Game Ogre listener
 */
class GameMainMenuState : public Ogre::FrameListener {
private:
	class MenuCallback : public GameMenuCallback {
		GameMainMenuState* m_pOwner;
	public:
		MenuCallback(GameMainMenuState *pOwner) : m_pOwner(pOwner) {}
		virtual void OnSwitchEvent() override;
		virtual void OnExitEvent() override;
		virtual void OnDebugEvent() override;
	};
private:
	GameClientEngine *m_pOwner;
	RefObjSmPtr<GameCompManager> m_spCompManager;
	RefObjSmPtr<GameMenu> m_spMenu;
	
	
	MenuCallback *m_pMenuClbk;
	GameState m_nextState;
	wxString m_nextStateParams;
	bool m_exitState;
	bool m_switchPolyMode;
	CDebugDraw* m_pDebugDraw;
	
	wxInt32 m_zeroTimeFrames;
	float m_timeSinceLastAverage;
	float m_averageFrameTime;
	
	
public:
	GameMainMenuState(GameClientEngine *pOwner, GameCompManager *pCompMgr) : m_pOwner(pOwner)
																		, m_spCompManager(pCompMgr)
																		, m_pMenuClbk(nullptr)
																		, m_nextState(GAME_STATE_EXIT)
																		, m_exitState(false)
																		, m_switchPolyMode(false)
																		, m_pDebugDraw(nullptr)
																		, m_zeroTimeFrames(0)
																		, m_timeSinceLastAverage(0)
																		, m_averageFrameTime(0){}
	
																		
	GameErrorCode ProcessState(GameState &nextState, wxString &nextStateParams );

	// Ogre::FrameListener event
	virtual bool frameStarted (	const Ogre::FrameEvent & evt ) override;	
	virtual bool frameRenderingQueued ( const Ogre::FrameEvent& evt ) override;
	virtual bool frameEnded	( const Ogre::FrameEvent & evt ) override;
	
	
	// input system callbacks
	void SetExitInputClbk(bool exit);
	void ChangeView();
	void SwitchPhysicsDebug();
	
	
	
	GameErrorCode ProcessUpdate( float secDiff);

};



#endif //__GAME_MAIN_MENU_STATE_H__