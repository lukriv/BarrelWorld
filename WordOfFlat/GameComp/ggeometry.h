#ifndef __GAME_GEOMETRY_OBJECT_H__
#define __GAME_GEOMETRY_OBJECT_H__

#include <sfml/graphics/Drawable.hpp>
#include <sfml/graphics/Color.hpp>
#include <Box2D/Collision/b2Collision.h>
#include <wx/vector.h>
#include "../GameSystem/gerror.h"

enum GameGeometryType {
	GAME_GEOM_UNDEFINED = 0,
	GAME_GEOM_POINTS,
	GAME_GEOM_LINES,
	GAME_GEOM_LINE_LOOP,
	GAME_GEOM_LINE_STRIP,
	GAME_GEOM_TRIANGLES,
	GAME_GEOM_TRIANGLE_STRIP,
	GAME_GEOM_TRIANGLE_FAN,
	GAME_GEOM_QUADS,
	GAME_GEOM_QUAD_STRIP,
	GAME_GEOM_POLYGON
};

struct GameGeometryContainer {
	GameGeometryType m_type;
	wxVector<b2Vec2> m_vertexes;
	wxVector<b2Vec2> m_texCoords;
	wxVector<sf::Color> m_colors;
public:
	GameGeometryContainer() : m_type(GAME_GEOM_UNDEFINED) {}
	~GameGeometryContainer() {
		m_vertexes.clear();
		m_texCoords.clear();
		m_colors.clear();
	}
	
	b2Shape* CreatePhysShape() const;
	b2AABB ComputeAABB() const;
	
};


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
	
	virtual GameErrorCode Create(const GameGeometryContainer& geomCont) = 0;
	
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