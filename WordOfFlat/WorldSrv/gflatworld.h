#ifndef __GAME_FLAT_WORLD_SERVER_H__
#define __GAME_FLAT_WORLD_SERVER_H__

#include "../GameSystem/gerror.h"
#include "../GameObjects/gobjdef.h"

typedef wxDword GameFlatWorldID;

static const GameFlatWorldID GAME_FLAT_WRLD_UNKNOWN = 0;


struct GameUpdateStruct {
	wxDword m_category;
};

/*! This could store world object for 2D
 *  It could simulate this part of world
 *  It could send updated objects from world
 * 
 */
class IGameFlatWorld {
public:
	
	virtual GameFlatWorldID GetFWId() = 0;
	
	virtual GameErrorCode SimulationStep() = 0;
	virtual GameErrorCode DrawStep() = 0;
	virtual GameErrorCode AIStep() = 0;
	
	virtual GameErrorCode GetUpdateList(GameUpdateStruct** &updList, wxDword &listSize) = 0;
	
};

#endif //__GAME_FLAT_WORLD_SERVER_H__