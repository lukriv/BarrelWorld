#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <wx/atomic.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/glog.h>
#include <GameComp/gcompmgr.h>
#include "gdefholder.h"



class GameEntityFactory : public RefObjectImpl<IRefObject> {
private:
	GameLoggerPtr m_spLogger;
public:
	GameEntityFactory(GameLogger *pLogger) : m_spLogger(pLogger) {}
	
	GameErrorCode CreateTerrain(GameDefinitionHolder &defHolder, GameCompManager& compMgr);
	
	GameErrorCode CreateAllEntities(GameDefinitionHolder &defHolder, GameCompManager& compMgr);
	
	GameErrorCode CreateEntity( EntityDef &entityDef, GameCompManager& compMgr, GameEntity *&pEntity);
	
};



#endif //__GAME_ENTITY_FACTORY_H__