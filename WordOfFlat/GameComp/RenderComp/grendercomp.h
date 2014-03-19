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
class RenderComponent {
	Ogre::Entity *m_pOgreEntity;
public:
	RenderComponent() : m_entity(NULL) {}
	
	~GameRenderComp() {}
	
	inline void SetEntity(Ogre::Entity *pOgreEntity) { m_pOgreEntity = pOgreEntity; }
	inline Ogre::Entity* GetEntity() { return m_pOgreEntity; }
	
};


#endif //__GAME_RENDER_COMPONENT_H__