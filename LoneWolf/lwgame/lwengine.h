#ifndef __LONE_WOLF_GAME_ENGINE_H__
#define __LONE_WOLF_GAME_ENGINE_H__


class LWGameEngineCallback {
public:
		
};


class LWGameEngine {
	Scene* m_pActualScene;
public:
	LWGameEngine() {}
	~LWGameEngine() {}
	
	
	void RunAction(wxDword actionIndex);
};



#endif //__LONE_WOLF_GAME_ENGINE_H__