#ifndef __GAME_MAIN_MENU_STATE_H__
#define __GAME_MAIN_MENU_STATE_H__


/**
 * \brief Game Ogre listener
 */
struct GameMainMenuState : public Ogre::FrameListener {
private:
	class MenuCallback : public GameMenuCallback {
		GameMainMenuState* m_pOwner;
	public:
		MenuCallback(GameMainMenuState *pOwner) : m_pOwner(pOwner) {}
		virtual void OnExitEvent() override;
	};
private:
	GameClientEngine *m_pOwner;
	RefObjSmPtr<GameCompManager> m_spCompManager;
	RefObjSmPtr<GameMenu> m_spMenu;
	
	MenuCallback *m_pMenuClbk;
	GameState m_nextState;
	wxString m_nextStateParams;
	bool m_exitState;
	
public:
	GameRenderListener(GameClientEngine *pOwner, GameCompManager *pCompMgr) : m_pOwner(pOwner)
																		, m_spCompManager(pCompMgr)
																		, m_pMenuClbk(nullptr)
																		, m_nextState(GAME_STATE_EXIT)
																		, m_exitState(false){}
	
																		
	GameErrorCode ProcessState(GameState &nextState, wxString &nextStateParams );

	// Ogre::FrameListener event
	virtual bool frameStarted (	const Ogre::FrameEvent & evt ) override;	
	virtual bool frameRenderingQueued ( const Ogre::FrameEvent& evt ) override;
	virtual bool frameEnded	( const Ogre::FrameEvent & evt ) override;
	
	
	// input system callbacks
	void SetExitInputClbk(bool exit);
	
	
	
	GameErrorCode ProcessUpdate( float secDiff);

};



#endif //__GAME_MAIN_MENU_STATE_H__