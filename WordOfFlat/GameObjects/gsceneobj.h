#ifndef __GAME_SCENE_OBJECT_H__
#define __GAME_SCENE_OBJECT_H__

#include "gobjbase.h"
#include <sfml/graphics/Drawable.hpp>
#include <Box2D/Box2D.h>
#include "../GameSystem/glog.h"
/*! \brief Object used for drawing scene graph
 */ 
class GameSceneObject: public GameObjectBase, public sf::Drawable {
	GameSceneObject* m_pParentObj;
	b2AABB m_AABB;
	
public:
	GameSceneObject() : m_pParentObj(NULL) {}
	inline GameSceneObject* GetParentObj() { return m_pParentObj;}
	inline void SetParentObj(GameSceneObject* pParent) {m_pParentObj = pParent;}
	inline b2AABB& GetAABB() { return m_AABB;}
		
public:
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual bool IsEntity() = 0;
	virtual void DebugInfo(GameLogger* pLogger) = 0;
};


#endif //__GAME_SCENE_OBJECT_H__