#ifndef __LONE_WOLF_GAME_ENGINE_H__
#define __LONE_WOLF_GAME_ENGINE_H__

#include <wx/defs.h>
#include "scenemgr.h"

class LWGameEngineCallback {
public:
		
};


class LWGameEngine {
	Scene* m_pActualScene;
	SceneManager m_sceneMgr;
	
public:
	LWGameEngine() {}
	~LWGameEngine() {}
	
	bool Initialize();
	
	inline Scene* GetActualScene() { return m_pActualScene;}
	
	void RunAction(wxDword actionIndex);
};



#endif //__LONE_WOLF_GAME_ENGINE_H__