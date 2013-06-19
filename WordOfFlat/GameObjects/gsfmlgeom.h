#ifndef __GAME_SFML_GEOMETRY_OBJECT_H__
#define __GAME_SFML_GEOMETRY_OBJECT_H__

#include "ggeometry.h"

/*!
 * \class GameGeometry 
 * \author Lukas
 * \date 11.6.2013
 * \file ggeometry.h
 * \brief Basic game geometry interface
 */
class GameSFMLGeometry : public IGameGeometry {
	sf::VertexArray *m_pVertexArray;
	b2AABB m_AABB;
private:
	void SetAABB() {
		sf::FloatRect boundBox = m_pVertexArray->getBounds();
		m_AABB.lowerBound.x = boundBox.left;
		m_AABB.lowerBound.y = boundBox.top - boundBox.height;
		m_AABB.upperBound.x = boundBox.left + boundBox.width;
		m_AABB.upperBound.y = boundBox.top;
	}

public:
	GameSFMLGeometry() : m_pVertexArray(nullptr) {}
	GameSFMLGeometry(sf::VertexArray *pVerArr) : m_pVertexArray(pVerArr) {
		SetAABB();
	}	
	virtual ~GameSFMLGeometry {
		if(m_pVertexArray != nullptr) {
			delete m_pVertexArray;
			m_pVertexArray = nullptr;
		}
	}
	
	void SetGeometry(sf::VertexArray *pVerArr) {
		if (m_pVertexArray != nullptr) {
			delete m_pVertexArray;
		}
		
		m_pVertexArray = pVerArr;
		SetAABB();
	}
	
	const b2AABB& GetAABB() const { return m_AABB;}
	/*! \brief Draw geometry to RenderTarget
	 * 
	 * It draw geometry from list. If list does not exists, it draw vertexes directly.
	 * 
	 * \param target
	 * \param states
	 */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const 
	{
		target.draw(*m_pVertexArray, states);
	}
};

#endif //__GAME_SFML_GEOMETRY_OBJECT_H__