#ifndef __GAME_FLAT_WORLD_SERVER_H__
#define __GAME_FLAT_WORLD_SERVER_H__

#include <scopedptr.h>
#include "../GameSystem/gerror.h"
/*! This could store world object for 2D
 *  It could add new object. 
 *  It could simulate this part of world
 *  It could send updated objects from world
 * 
 */
 
enum GameFlatWorldID {
	GAME_FLAT_WRLD_UNKNOWN = 0,
	GAME_FLAT_WRLD_TEST = 1
};

class IFlatLocalWorldSrv : IRefObject {
public:
	
	virtual GameFlatWorldID GetFWId() = 0;
	
	virtual GameErrorCode StepWorld() = 0;
	
	virtual GameErrorCode AddNewObject(IGameObjectSrv *object) = 0;
	virtual GameErrorCode SetWorldSize(const b2Vec2 &LLpoint, const b2Vec2 &RUpoint) = 0;
	
	virtual GameErrorCode LoadWorld(const wxChar* worldName)
	
}

#endif //__GAME_FLAT_WORLD_SERVER_H__