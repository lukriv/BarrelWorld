#ifndef __GAME_RESOURCE_HOLDER_H__
#define __GAME_RESOURCE_HOLDER_H__

class GameResourceHolder {
	static GameResourceHolder* g_pResourceHolder;
private:
	bool m_isInitialized;
private:
	GameResourceHolder(): m_isInitialized(false)
	{}
public:
	static GameResourceHolder* GetResourceHolder();
	~GameResourceHolder();
};


#endif //__GAME_RESOURCE_HOLDER_H__