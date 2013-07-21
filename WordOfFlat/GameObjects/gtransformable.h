#ifndef __GAME_TRASFORMABLE_H__
#define __GAME_TRASFORMABLE_H__

#include <sfml/system/Vector2.hpp>
#include <sfml/graphics/Transformable.hpp>
#include <box2d/common/b2Math.h>
#include "gameobjdef.h"


class GameTransformable : public sf::Transformable {
public:
	inline void SetPosition(float x, float y) {
		setPosition(Pixelize * x, -(Pixelize * y));
	}
	
	inline void SetOrigin(float x, float y) {
		setOrigin(Pixelize * x, -(Pixelize * y));
	}
	
	inline void SetScale(float factorX, float factorY) {
		setScale(Pixelize * factorX, Pixelize * factorY);
	}
	
	inline void SetRotation(float rot) {
		setRotation(-(rot/b2_pi)*180);
	}
	
	inline b2Vec2 GetPosition() {
		sf::Vector2f sfVec(getPosition());
		return b2Vec2(sfVec.x/Pixelize, -(sfVec.y/Pixelize));
	}
	
	inline b2Vec2 GetOrigin() {
		sf::Vector2f sfVec(getOrigin());
		return b2Vec2(sfVec.x/Pixelize, -(sfVec.y/Pixelize));
	}
	
	inline b2Vec2 GetScale() {
		sf::Vector2f sfVec(getScale());
		return b2Vec2(sfVec.x/Pixelize, sfVec.y/Pixelize);
	}
	
	inline float GetRotation() {
		return (-(getRotation()/180.0f)*b2_pi);
	}
};



#endif //__GAME_TRASFORMABLE_H__