#ifndef __GAME_GEOMETRY_OBJECT_H__
#define __GAME_GEOMETRY_OBJECT_H__

#include <sfml/graphics/Drawable.hpp>
#include <Box2D/Collision/b2Collision.h>
/*!
 * \class GameGeometry 
 * \author Lukas
 * \date 11.6.2013
 * \file ggeometry.h
 * \brief Basic game geometry interface
 */
class IGameGeometry : public sf::Drawable {
public:
	virtual ~IGameGeometry() {}
	
	virtual const b2AABB& GetAABB() const = 0;
	
	virtual b2Shape* CreatePhysShape() const = 0;
	
	/*! \brief Draw geometry to RenderTarget
	 * 
	 * It draw geometry from list. If list does not exists, it draw vertexes directly.
	 * 
	 * \param target
	 * \param states
	 */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

#endif //__GAME_GEOMETRY_OBJECT_H__