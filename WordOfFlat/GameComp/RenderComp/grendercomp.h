#ifndef __GAME_RENDER_COMPONENT_H__
#define __GAME_RENDER_COMPONENT_H__

#include <OGRE/OgreEntity.h>


/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class RenderComponent : public Ogre::Any {
	GameEntity *m_pParent;
	Ogre::Entity *m_pOgreEntity;
public:
	RenderComponent() : m_pParent(NULL), m_pOgreEntity(NULL) {}
	
	~GameRenderComp() {}
	
	inline void SetEntity(Ogre::Entity *pOgreEntity) 
	{ 
		if(m_pOgreEntity != NULL)
		{
			m_pOgreEntity->getUserObjectBindings().setUserAny(Ogre::UserObjectBindings::getEmptyUserAny()); // erase parent
		}
		
		m_pOgreEntity = pOgreEntity; // set new entity
		
		if(pOgreEntity != NULL)
		{
			m_pOgreEntity->getUserObjectBindings().setUserAny(this); // set parent
		}
	}
	
	inline Ogre::Entity* GetEntity() { return m_pOgreEntity; }
	
	inline void SetParent(GameEntity *pParent) { m_pParent = pParent; }
	inline GameEntity* GetParent() { return m_pParent; }
	
};


#endif //__GAME_RENDER_COMPONENT_H__