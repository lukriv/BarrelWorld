#ifndef __GAME_ENTITY_MANAGER_H__
#define __GAME_ENTITY_MANAGER_H__

#include "../GameSystem/gmap.h"
#include "gentity.h"

class GameEntityManager {
	typedef GameBasMap<wxString, GameEntity> TEntityMap;
private:
	TEntityMap m_entityMap;
public:
	
	
	GameEntity* CreateEntity(const wxString& entityName);
	GameEntity* GetEntity(const wxString& entityName);
	void RemoveEntity(const wxString& entityName);
	
};


#endif //__GAME_ENTITY_MANAGER_H__