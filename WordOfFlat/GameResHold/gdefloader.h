#ifndef __GAME_DEFINITIONS_LOADER_H__
#define __GAME_DEFINITIONS_LOADER_H__


#include <GameSystem/gerror.h>

class GameCompManager;

class IGameDefLoader {
public:
	virtual ~IGameDefLoader() {}
	virtual GameErrorCode Load(GameCompManager &compMgr) = 0;
	
};

#endif //__GAME_DEFINITIONS_LOADER_H__