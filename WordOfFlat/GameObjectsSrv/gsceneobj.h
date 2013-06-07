#ifndef __GAME_SCENE_OBJECT_H__
#define __GAME_SCENE_OBJECT_H__

#include "gameobjdef.h"
#include "gobject.h"
#include <sfml/graphics/Drawable.hpp>

class GameSceneObject: public GameObjectBase, public sf::Drawable {
	GameSceneObject* m_pParentObj;
	b2AABB m_AABB;
	
public:
	GameSceneObject() : m_pParentObj(nullptr) {}
	GameSceneObject* GetParentObj() { return m_pParentObj;}
	virtual b2AABB& GetAABB() { return m_AABB;}
	
public:
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const = 0;
}


#endif //__GAME_SCENE_OBJECT_H__