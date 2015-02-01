#ifndef __GAME_TERRAIN_ENTITY_H__
#define __GAME_TERRAIN_ENTITY_H__


#include <wx/thread.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/gmap.h>

#include "gcompbase.h"

class GameTerrainEntity {
private:
	wxCriticalSection m_entityLock;
	

public:

	GameTerrainEntity();
	~GameTerrainEntity();
				

	void SetOgreTerrain(Ogre::Terrain* pNewTerrain);

	/**
	 * \brief Get component with given type
	 * @return Component of given type or nullptr if compoent is not present
	 */
	Ogre::Terrain* GetOgreTerrain();
				
				
	void SetPhysicsTerrain(btHeightfieldTerrainShape);
	/**
	 * \brief Add component to entity
	 * @return Errorcode on error. 
	 */
	GetPhysicsTerrain();
	
	
	/**
	 * \brief Receive message from outside
	 * 
	 * Message can be sent to the selected compoents of the entity or for all components
	 * 
	 * @param msg Message task
	 * @param targetMask Component receivers
	 * @return 
	 */
	GameErrorCode ReceiveMessage(TaskMessage& msg, wxDword targetMask = GAME_COMP_MASK_ALL);
	
	/**
	 * \brief Update all components in the entity
	 * @return Errorcode on failture.
	 */
	GameErrorCode Update();
	
};




#endif // __GAME_TERRAIN_ENTITY_H__