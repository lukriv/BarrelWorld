#ifndef __GAME_RENDER_COMPONENT_ENTITY_H__
#define __GAME_RENDER_COMPONENT_ENTITY_H__


#include "grendercomp.h"


/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class RenderEntity : public RenderComponent {
protected:
	
	Ogre::Entity *m_pOgreEntity; //!< Main render object (it is optional)
public:
	// Render component can be created and destroyed only by render component manager
	RenderEntity(RenderCompManager* pCompManager) : RenderComponent(pCompManager)
													, m_pOgreEntity(NULL){}
	~RenderEntity();
	
	inline Ogre::Entity* GetOgreEntity() 
	{
		return m_pOgreEntity;
	}
	
	void SetOgreEntity(Ogre::Entity* pEntity);
	
	inline void SetParent(GameEntity *pParent) { m_pParent = pParent; }
	inline GameEntity* GetParent() { return m_pParent; }
	
	/*!
	 * \brief Destroy inner ogre object
	 */
	virtual void Clear() override;
	
	virtual void ConnectTransformComp(TransformComponent &transform) override;
	
};

#endif //__GAME_RENDER_COMPONENT_ENTITY_H__