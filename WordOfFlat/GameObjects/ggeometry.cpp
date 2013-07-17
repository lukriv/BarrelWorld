#include "ggeometry.h"

#include <wx/scopedptr.h>
#include <Box2D/Box2D.h>


b2Shape* GameGeometryContainer::CreatePhysShape() const
{
	switch(m_type)
	{
	case GAME_GEOM_POINTS:         ///< List of individual points
	case GAME_GEOM_LINES:          ///< List of individual lines
	case GAME_GEOM_LINE_LOOP:
	case GAME_GEOM_LINE_STRIP:     ///< List of connected lines, a point uses the previous point to form a line
    case GAME_GEOM_TRIANGLES:      ///< List of individual triangles
    case GAME_GEOM_TRIANGLE_STRIP: ///< List of connected triangles, a point uses the two previous points to form a triangle
    case GAME_GEOM_TRIANGLE_FAN:   ///< List of connected triangles, a point uses the common center and the previous point to form a triangle
		break;
		
    case GAME_GEOM_QUADS: 
		wxScopedPtr<b2PolygonShape> apPolyShape;
		apPolyShape.reset(new (std::nothrow) b2PolygonShape());
		b2Vec2 vertices[4];
		for (wxInt32 i = 0; i < 4; i++)
		{
			vertices[i].Set((m_vertexes[i].x, m_vertexes[i].y);
		}
		apPolyShape->Set(vertices, 4);
		return apPolyShape.release();
		
	case GAME_GEOM_QUAD_STRIP:
	case GAME_GEOM_POLYGON:
		break;
	}
	
	return NULL;
}
