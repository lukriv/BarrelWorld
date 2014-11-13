#ifndef __GAME_DEFINITIONS_LOADER_H__
#define __GAME_DEFINITIONS_LOADER_H__



#include "gdefholder.h"


class IGameDefLoader {
public:
	virtual ~IGameDefLoader() {}
	virtual GameErrorCode Load(GameDefinitionHolder &defHolder) = 0;
	
};

#endif //__GAME_DEFINITIONS_LOADER_H__