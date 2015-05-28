#ifndef __GAME_ENTITY_MANAGER_H__
#define __GAME_ENTITY_MANAGER_H__

#include <GameSystem/gmap.h>
#include <GameSystem/memalloccont.h>
#include <GameSystem/glog.h>
#include "gentity.h"

class GameEntityManager {
	//typedef GameBasMap<wxDword, GameEntity> TEntityMap;
private:
	GameLoggerPtr m_spLogger;
	MemoryAllocContainer<GameEntity> m_entityMap;
public:
	GameEntityManager(GameLogger *pLogger);
	~GameEntityManager();
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	GameErrorCode CreateEntity(GameEntity*& pNewEntity);
	void DestroyEntity(GameEntity* entity);
	void DestroyEntity(wxDword entityId);
	void DestroyAllEntities();
	
	GameEntity* GetEntity(wxDword entityId);
	
	
};


#endif //__GAME_ENTITY_MANAGER_H__