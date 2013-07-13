#include "gsfmlgeom.h"

#include <wx/scopedptr.h>
#include <Box2D/Box2D.h>

b2Shape* GameSFMLGeometry::CreatePhysShape() const
{
	switch(m_pVertexArray->getPrimitiveType())
	{
	case sf::Points:         ///< List of individual points
	case sf::Lines:          ///< List of individual lines
	case sf::LinesStrip:     ///< List of connected lines, a point uses the previous point to form a line
    case sf::Triangles:      ///< List of individual triangles
    case sf::TrianglesStrip: ///< List of connected triangles, a point uses the two previous points to form a triangle
    case sf::TrianglesFan:   ///< List of connected triangles, a point uses the common center and the previous point to form a triangle
		break;
    case sf::Quads: 
		wxScopedPtr<b2PolygonShape> apPolyShape;
		apPolyShape.reset(new (std::nothrow) b2PolygonShape());
		b2Vec2 vertices[4];
		for (wxInt32 i = 0; i < 4; i++)
		{
			vertices[i].Set((*m_pVertexArray)[i].position.x, (*m_pVertexArray)[i].position.y);
		}
		apPolyShape->Set(vertices, 4);
		return apPolyShape.release();
	}
	
	return NULL;
}
