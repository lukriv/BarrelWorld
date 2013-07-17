#ifndef __GAME_GENERIC_GEOMETRY_H__
#define __GAME_GENERIC_GEOMETRY_H__

#include <GL/gl.h>
#include <wx/vector.h>
#include "../GameSystem/gerror.h"
#include "ggeometry.h"

/*! \brief Geometry types
 * 
 *  Reflects OpenGL geometry types. Involve vertex order.
 */ 
 inline GameGeometryType ConversionGlPrimitives2GameGeomType(wxInt32 glPrim)
 {
	 switch (glPrim)
	 {
	 case GL_POINTS:
		return GAME_GEOM_POINTS;
	 case GL_LINES:
		return GAME_GEOM_LINES;
	 case GL_LINE_LOOP:
		return GAME_GEOM_LINE_LOOP;
	 case GL_LINE_STRIP:
		return GAME_GEOM_LINE_STRIP;
	 case GL_TRIANGLES:
		return GAME_GEOM_TRIANGLES;
	 case GL_TRIANGLE_STRIP:
		return GAME_GEOM_TRIANGLE_STRIP;
	 case GL_TRIANGLE_FAN:
		return GAME_GEOM_TRIANGLE_FAN;
	 case GL_QUADS:
		return GAME_GEOM_QUADS;
	 case GL_QUAD_STRIP:
		return GAME_GEOM_QUAD_STRIP;
	 case GL_POLYGON:
		return GAME_GEOM_POLYGON;
	 default:
		return GAME_GEOM_UNDEFINED;
	 }
 }



/*!
 * \class GameGeometry 
 * \author Lukas
 * \date 11.6.2013
 * \file ggeometry.h
 * \brief Basic game geometry
 */
class GameGLGeometry : public IGameGeometry {
	GameGeometryType m_geomType;
	wxVector<b2Vec2> m_vertexList;
	wxVector<b2Vec2> m_texCoordsList;
	wxVector<b2Vec3> m_normalList;
	GLuint m_glList;
	b2AABB m_AABB;
private:
	void DeleteList();
public:
	GameGLGeometry(GameGeometryType geomType) : m_geomType(geomType), m_glList(0) {}
	virtual ~GameGLGeometry();
	
	GameGeometryType GetType() { return m_geomType;}
	
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