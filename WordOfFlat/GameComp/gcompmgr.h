#ifndef __GAME_COMPONENT_MANAGER_H__
#define __GAME_COMPONENT_MANAGER_H__

#include "gentitymgr.h"

class GameCompManager
{
public:
	GameCompManager();
	~GameCompManager();
	
	GameEntityManager m_entityMgr;

};

#endif // __GAME_COMPONENT_MANAGER_H__
