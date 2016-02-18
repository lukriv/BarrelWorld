#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/glog.h>
#include <GameComp/gcompmgr.h>
#include <GameComp/TerrainComp/gterraindef.h>



class GameEntityFactory : public RefObjectImpl<IRefObject> {
private:
	GameLoggerPtr m_spLogger;
public:
	GameEntityFactory(GameLogger *pLogger) : m_spLogger(pLogger) {}
	
	GameErrorCode CreateFloor(GameCompManager& compMgr);
	
	GameErrorCode CreateMainCamera(GameCompManager& compMgr);
	
	
	
};



#endif //__GAME_ENTITY_FACTORY_H__