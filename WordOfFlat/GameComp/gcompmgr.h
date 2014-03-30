#ifndef __GAME_COMPONENT_MANAGER_H__
#define __GAME_COMPONENT_MANAGER_H__

#include "gentitymgr.h"

class GameCompManager {
	GameEntityManager m_entityMgr;
public:
	GameCompManager();
	~GameCompManager();
	
	
	
	GameEntityManager& GetEntityManager() { return m_entityMgr; }

};

#endif // __GAME_COMPONENT_MANAGER_H__
