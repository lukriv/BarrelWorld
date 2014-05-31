#ifndef __GAME_ENTITY_MANAGER_H__
#define __GAME_ENTITY_MANAGER_H__

#include <GameSystem/gmap.h>
#include <GameSystem/glog.h>
#include "gentity.h"

class GameEntityManager {
	typedef GameBasMap<wxString, GameEntity> TEntityMap;
private:
	GameLoggerPtr m_spLogger;
	TEntityMap m_entityMap;
public:
	GameEntityManager(GameLogger *pLogger);
	~GameEntityManager();
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	GameErrorCode CreateEntity(const wxString& entityName, GameEntity*& pNewEntity);
	void DestroyEntity(GameEntity* entity);
	void DestroyEntity(const wxString& entityName);
	void DestroyAllEntities();
	
	GameEntity* GetEntity(const wxString& entityName);
	
	
};


#endif //__GAME_ENTITY_MANAGER_H__