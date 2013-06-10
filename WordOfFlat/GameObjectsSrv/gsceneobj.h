#ifndef __GAME_SCENE_OBJECT_H__
#define __GAME_SCENE_OBJECT_H__

#include "gobjbase.h"
#include <sfml/graphics/Drawable.hpp>

/*! \brief Object used for drawing scene graph
 */ 
class GameSceneObject: public GameObjectBase, public sf::Drawable {
	GameSceneObject* m_pParentObj;
	b2AABB m_AABB;
	
public:
	GameSceneObject() : m_pParentObj(nullptr) {}
	inline GameSceneObject* GetParentObj() { return m_pParentObj;}
	inline b2AABB& GetAABB() { return m_AABB;}
	inline wxUint32 GetLeafCount();
	
public:
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const = 0;
}


#endif //__GAME_SCENE_OBJECT_H__