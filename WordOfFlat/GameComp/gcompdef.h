#ifndef __GAME_COMPONENT_DEFINITIONS_H__
#define __GAME_COMPONENT_DEFINITIONS_H__

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
};



#endif //__GAME_COMPONENT_DEFINITIONS_H__