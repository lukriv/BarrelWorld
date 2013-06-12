#ifndef __GAME_GENERIC_GEOMETRY_H__
#define __GAME_GENERIC_GEOMETRY_H__

#include "ggeometry.h"

/*! \brief Geometry types
 * 
 *  Reflects OpenGL geometry types. Involve vertex order.
 */ 
enum GameGometryType {
	GAME_POINTS = GL_POINTS,
	GAME_LINES = GL_LINES,
	GAME_LINE_LOOP = GL_LINE_LOOP,
	GAME_LINE_STRIP = GL_LINE_STRIP,
	GAME_TRIANGLES = GL_TRIANGLES,
	GAME_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
	GAME_TRIANGLE_FAN = GL_TRIANGLE_FAN,
	GAME_QUADS = GL_QUADS,
	GAME_QUAD_STRIP = GL_QUAD_STRIP,
	GAME_POLYGON = GL_POLYGON
};


/*!
 * \class GameGeometry 
 * \author Lukas
 * \date 11.6.2013
 * \file ggeometry.h
 * \brief Basic game geometry
 */
class GameGLGeometry : public IGameGeometry {
	GameGometryType m_geomType;
	wxVector<b2Vec2> m_vertexList;
	wxVector<b2Vec2> m_texCoordsList;
	wxVector<b2Vec3> m_normalList;
	GLuint m_glList;
	b2AABB m_AABB;
private:
	void DeleteList();
public:
	GameGLGeometry(GameGometryType geomType) : m_geomType(geomType), m_glList(0) {}
	virtual ~GameGLGeometry();
	
	GameGometryType GetType() { return m_geomType;}
	
	const b2AABB& GetAABB() const {return m_AABB;}
	
	
	/*! \brief Add vertex with texture coordiantes 
	 * 
	 *  Add vertex and update AABB.
	 * \param vertex Vertex coords related to geometry origin
	 * \param texCoords Texture coords
	 */
	void AddVertex(const b2Vec2 &vertex, const b2Vec2 &texCoords);
	
	/*! \brief Add normal to vertex with same index 
	 * 
	 * This is normal for vertex with same index (probably right added vertex).
	 * If this vector is empty, all normals will be the same [0.0, 0.0, -1.0].
	 * If vector has less normals than vectors, last normal will be used for rest of vectors with no normals.
	 * 
	 * \param normal
	 */
	void AddNormal(const b2Vec3 &normal);
	
	/*! \brief Create gllist of this geometry
	 * 
	 * This method should deletes previously created gllist (if exists) and create new one from stored vertex.
	 * 
	 * \retval FWG_NO_ERROR On success
	 * \retval Other error code on fail
	 */
	GameErrorCode CreateList();
	
	
	/*! \brief Draw geometry to RenderTarget
	 * 
	 * It draw geometry from list. If list does not exists, it draw vertexes directly.
	 * 
	 * \param target
	 * \param states
	 */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif //__GAME_GENERIC_GEOMETRY_H__