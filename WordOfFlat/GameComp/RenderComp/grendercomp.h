#ifndef __GAME_RENDER_COMPONENT_H__
#define __GAME_RENDER_COMPONENT_H__

#include <OGRE/OgreEntity.h>

class GameEntity;

/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class RenderComponent : public Ogre::Any {
	GameEntity *m_pParent;
	Ogre::MovableObject *m_pOgreObject;
public:
	RenderComponent() : m_pParent(NULL), m_pOgreObject(NULL) {}
	
	~RenderComponent() {}
	
	inline void SetOgreObject(Ogre::MovableObject *pOgreObject) 
	{ 
		if(m_pOgreObject != NULL)
		{
			m_pOgreObject->getUserObjectBindings().setUserAny(Ogre::UserObjectBindings::getEmptyUserAny()); // erase parent
		}
		
		m_pOgreObject = pOgreObject; // set new entity
		
		if(pOgreObject != NULL)
		{
			m_pOgreObject->getUserObjectBindings().setUserAny(*this); // set parent
		}
	}
	
	inline Ogre::MovableObject* GetOgreObject() { return m_pOgreObject; }
	
	inline void SetParent(GameEntity *pParent) { m_pParent = pParent; }
	inline GameEntity* GetParent() { return m_pParent; }
	
};


#endif //__GAME_RENDER_COMPONENT_H__