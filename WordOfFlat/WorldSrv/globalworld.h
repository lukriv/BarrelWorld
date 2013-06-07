#ifndef __GAME_GLOBAL_WORLD_SERVER_H__
#define __GAME_GLOBAL_WORLD_SERVER_H__

#include "gflatworld.h"

class IGlobalWorldSrv {
	
	virtual GameErrorCode GenerateTestStaticWorld() = 0;
	
	
	/*!
	 * \brief Get flat world 
	 * 
	 *  It returns flat world (physics world) with given id.
	 * Return nullptr if flatworld with given id does not exists.
	 *  
	 * 
	 * \param wrldId World ID
	 * \param pFlatWorld Pointer to physics simulation flatworld.
	 * \retval FWG_NO_ERROR when succeded
	 * 
	 */
	virtual GameErrorCode GetFlatWorld(GameFlatWorldID wrldId, IGameFlatWorldSrv *& pFlatWorld ) = 0;
	
}

#endif //__GAME_GLOBAL_WORLD_SERVER_H__