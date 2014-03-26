#ifndef __GAME_ENTITY_MANAGER_H__
#define __GAME_ENTITY_MANAGER_H__

#include "../GameSystem/gmap.h"
#include "gentity.h"

class GameEntityManager {
public:
	GameBasMap<wxString, GameEntity> m_entityMap;
	
};


#endif //__GAME_ENTITY_MANAGER_H__