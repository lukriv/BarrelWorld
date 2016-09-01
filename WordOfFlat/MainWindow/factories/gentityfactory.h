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
	
	GameErrorCode CreateMainCamera(GameCompManager& compMgr, wxDword &mainCamIndex);
	
	GameErrorCode CreateBox(GameCompManager& compMgr, wxDword &boxid, const btVector3& place = btVector3(0,10,0));
	
	GameErrorCode CreateStaticBox(GameCompManager& compMgr, wxDword &boxid, const btVector3& place = btVector3(0,1,0));
	
	GameErrorCode CreateAvatar(GameCompManager& compMgr, wxDword &avatarIndex, const btVector3& place = btVector3(0,2.0f,0));
	
	GameErrorCode CreateBullet(GameCompManager& compMgr, wxDword &bulletid, const btVector3& place = btVector3(0,2.0f,0));
	
};



#endif //__GAME_ENTITY_FACTORY_H__