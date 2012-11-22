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

class IFlatLocalWorldSrv {
public:
	
	GameErrorCode Initialize();
	GameErrorCode AddNewObject(IGameObjectSrv *object);
	GameErrorCode SetWorldSize(const b2Vec2 &LLpoint, const b2Vec2 &RUpoint);
	GameErrorCode GenerateTestStaticWorld();
	
}

#endif //__GAME_FLAT_WORLD_SERVER_H__