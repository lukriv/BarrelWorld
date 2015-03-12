#ifndef __DYNAMIC_ENTITY_CREATOR_H__
#define __DYNAMIC_ENTITY_CREATOR_H__

#include <wx/atomic.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/glog.h>
#include "gdeftables.h"


/**
 * @class GameEntityCreator
 * @author Lukas
 * @date 03/01/15
 * @file gentitycreator.h
 * @brief Create random definition of diferent game object
 * 
 * Creates e.g. entity definitions for diferent game object
 * 
 */
class GameEntityCreator : public RefObjectImpl<IRefObject> {
private:
	GameLoggerPtr m_spLogger;
public:
	GameEntityCreator(GameLogger *pLogger) : m_spLogger(pLogger) {}
	
	GameErrorCode CreateTerrainDecal(GameCompManager& compMgr, GameEntity *&pTerrDecal);

};


#endif //__DYNAMIC_ENTITY_CREATOR_H__