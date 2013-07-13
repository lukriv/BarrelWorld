#include "ggengeom.h"

void GameGLGeometry::AddVertex(const b2Vec2& vertex, const b2Vec2& texCoords)
{
	m_vertexList.push_back(vertex);
	m_texCoordsList.push_back(texCoords);
	b2AABB aabb;
	aabb.lowerBound = vertex;
	aabb.upperBound = vertex;
	if(m_vertexList.size() == 1) {
		m_AABB = aabb;
	} else {
		m_AABB.Combine(aabb);
	}
}

GameErrorCode GameGLGeometry::CreateList()
{
	wxDword normSize = m_normalList.size();
	DeleteList();
	m_glList = glGenLists(1);
	glNewList(m_glList, GL_COMPILE);
		glBegin(m_geomType);
		for (wxDword i = 0; i < m_vertexList.size(); ++i)
		{
			if (normSize > i) {
				const b2Vec3& norm = m_normalList[i];
				glNormal3f(norm.x, norm.y, norm.z);
			}
			glTexCoord2f(m_texCoordsList[i].x, m_texCoordsList[i].y);
			glVertex2f(m_vertexList[i].x, m_vertexList[i].y);
		}
		glEnd();
	glEndList();
	
	return FWG_NO_ERROR;
}

void GameGLGeometry::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	FWG_UNREFERENCED_PARAMETER(target);
	FWG_UNREFERENCED_PARAMETER(states);
	/*glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadMatrixf(states.transform.getMatrix());

	if (m_glList == 0) {
		glBegin(m_geomType);
		if(states.texture != NULL) {
			sf::Texture::bind(states.texture ); //bind normalized texture
		}
		for (wxDword i = 0; i < m_vertexList.size; ++i)
		{
			if (m_normalList.size() > i) {
				const b2Vec3& norm = m_normalList[i];
				glNormal3f(norm.x, norm.y, norm.z);
			}
			glTexCoord2f(m_texCoordsList[i].x, m_texCoordsList[i].y);
			glVertex2f(m_vertexList[i].x, m_vertexList[i].y);
		}
		glEnd();
	} else {
		glCallList(m_glList);
	}*/
}

GameGLGeometry::~GameGLGeometry()
{
	DeleteList();
}

void GameGLGeometry::DeleteList()
{
	if(m_glList != 0)
	{
		glDeleteLists(m_glList,1);
		m_glList = 0;
	}
}

void GameGLGeometry::AddNormal(const b2Vec3& normal)
{
	m_normalList.push_back(normal);
}
