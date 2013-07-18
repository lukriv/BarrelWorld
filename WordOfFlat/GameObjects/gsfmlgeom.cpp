#include "gsfmlgeom.h"
#include "gameobjdef.h"



GameErrorCode GameSFMLGeometry::Create(const GameGeometryContainer& geomCont)
{
	m_vertexArray.clear();
	
	switch(geomCont.m_type)
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
		{	
			// set type
			m_vertexArray.setPrimitiveType(sf::Quads);
			m_vertexArray.resize(4);
			wxDword i = 0;
			for(i = 0; i < 4;i++)
			{
				if(geomCont.m_vertexes.size() > i)
				{
					m_vertexArray[i].position = sf::Vector2f(Pixelize * geomCont.m_vertexes[i].x , -(Pixelize * geomCont.m_vertexes[i].y));
				}
				
				if(geomCont.m_texCoords.size() > i)
				{
					m_vertexArray[i].texCoords = sf::Vector2f(geomCont.m_texCoords[i].x, geomCont.m_texCoords[i].y);
				}
				
				if (geomCont.m_colors.size() > i)
				{
					m_vertexArray[i].color = geomCont.m_colors[i];
				}
			}

			return FWG_NO_ERROR;
		}	
		case GAME_GEOM_QUAD_STRIP:
		case GAME_GEOM_POLYGON:
		case GAME_GEOM_UNDEFINED:
			break;
	}
	
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}
