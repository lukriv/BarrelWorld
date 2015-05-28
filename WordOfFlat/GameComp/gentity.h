#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__


#include <wx/thread.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/gmap.h>

#include "gcompbase.h"

class GameEntity {
	typedef GameBasMap< GameComponentType, RefObjSmPtr< ComponentBase > > TCompMap;
private:
	wxDword m_id;
	wxCriticalSection m_entityLock;
	TCompMap m_componentList;

public:

	GameEntity();
	GameEntity(const GameEntity& master);
	
	~GameEntity();
				
	/**
	 * \brief Sets GameEntity unique id 
	 */
	inline void SetId(wxDword id) { m_id = id;}
	
	/**
	 * \brief Gets GameEntity unique name
	 */
	inline wxDword GetId() const { return m_id; }

	/**
	 * \brief Get component with given type
	 * 
	 * @param compType
	 * @return Component of given type or nullptr if compoent is not present
	 */
	ComponentBase* GetComponent(GameComponentType compType);
				
	/**
	 * \brief Add component to entity
	 * 
	 * If component type already exists within entity, it returns error.
	 * 
	 * @param pComp Pointer to new compoent
	 * @return Errorcode on error. 
	 */
	GameErrorCode AddComponent(ComponentBase *pComp);
	
	/**
	 * \brief Remove component with same compoent type from entity
	 * @param compType Compoent type to remove
	 * @return Error on fail.
	 */
	GameErrorCode RemoveComponent(GameComponentType compType);
	
	/**
	 * \brief Reinitialize components after entity reconfiguration 
	 * 
	 * When the components of the entity are changed ( removed or added ) it is necessary to call this reconfiguration.
	 * Some of the components may have connections on the old components (e.g. transform component on physics component) and
	 * the not works properly if this reconfiguration was not done.
	 * 
	 * @return Error on failure.
	 */
	GameErrorCode ReinitComponents();
	
	/**
	 * \brief Receive message from outside
	 * 
	 * Message can be sent to the selected compoents of the entity or for all components
	 * 
	 * @param msg Message task
	 * @param targetMask Component receivers
	 * @return 
	 */
	GameErrorCode ReceiveMessage(TaskMessage& msg);
	
	/**
	 * \brief Update all components in the entity
	 * @return Errorcode on failture.
	 */
	GameErrorCode Update();
	
	/**
	 * @brief Clear entity
	 */
	void Clear();
	
};




#endif // __GAME_ENTITY_H__
