#ifndef __GAME_RENDER_COMPONENT_ENTITY_H__
#define __GAME_RENDER_COMPONENT_ENTITY_H__


#include "grenderobj.h"


/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class RenderEntity : public RenderObject {
protected:
	
	Ogre::Entity *m_pOgreEntity; //!< Main render object (it is optional)
public:
	// Render component can be created and destroyed only by render component manager
	RenderEntity(Ogre::Entity* pOgreEntity) : RenderObject(RenderObject::RENDER_OBJECT_TYPE_ENTITY)
													, m_pOgreEntity(pOgreEntity){}
	~RenderEntity();
	
	inline Ogre::Entity* GetOgreEntity()
	{
		return m_pOgreEntity;
	}
	
	/*!
	 * \brief Destroy inner ogre object
	 */
	virtual Ogre::MovableObject* GetMovableObject() override;
	
};

#endif //__GAME_RENDER_COMPONENT_ENTITY_H__