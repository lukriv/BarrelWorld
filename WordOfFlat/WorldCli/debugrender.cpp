
#include "debugrender.h"
#include <sfml/graphics/Color.hpp>
#include <sfml/graphics/VertexArray.hpp>
#include <SFML/Config.hpp>
#include "../GameObjects/gameobjdef.h"

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	int32 i = 0;
	sf::Color sfcolor((sf::Uint8)(255.0f * color.r), (sf::Uint8)(255.0f * color.g), (sf::Uint8)(255.0f * color.b), 255);
	sf::VertexArray polygonLines(sf::LinesStrip, vertexCount + 1);
	for (i = 0; i < vertexCount; ++i)
	{
		polygonLines[i].position = sf::Vector2f(vertices[i].x * Pixelize, -(vertices[i].y * Pixelize));
		polygonLines[i].color = sfcolor;
	}
		
	polygonLines[i].position = sf::Vector2f(vertices[0].x * Pixelize, -(vertices[0].y * Pixelize));
	polygonLines[i].color = sfcolor;
	
	m_pRenderTarget->draw(polygonLines);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	/*glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();*/
	DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	int32 i = 0;
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	sf::Color sfcolor((sf::Uint8)(255.0f * color.r), (sf::Uint8)(255.0f * color.g), (sf::Uint8)(255.0f * color.b), 255);
	sf::VertexArray polygonLines(sf::LinesStrip, k_segments + 1);
	for (i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		polygonLines[i].position = sf::Vector2f(v.x * Pixelize, -(v.y * Pixelize));
		polygonLines[i].color = sfcolor;
		theta += k_increment;
	}
	b2Vec2 vend = center + radius * b2Vec2(cosf(theta), sinf(theta));
	polygonLines[i].position = sf::Vector2f(vend.x * Pixelize, -(vend.y * Pixelize));
	polygonLines[i].color = sfcolor;
	
	m_pRenderTarget->draw(polygonLines);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) 
{
	DrawCircle(center, radius, color);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	sf::Color sfcolor((sf::Uint8)(255.0f * color.r), (sf::Uint8)(255.0f * color.g), (sf::Uint8)(255.0f * color.b), 255);
	sf::VertexArray line(sf::Lines , 2);
	line[0].position = sf::Vector2f(p1.x * Pixelize, -(p1.y * Pixelize));
	line[0].color = sfcolor;
	
	line[1].position = sf::Vector2f(p2.x * Pixelize, -(p2.y * Pixelize));
	line[1].color = sfcolor;

	m_pRenderTarget->draw(line);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	sf::VertexArray line(sf::Lines , 4);
	
	sf::Color sfcolor(255, 0, 0, 255);
	line[0].position = sf::Vector2f(p1.x * Pixelize, -(p1.y * Pixelize));
	line[0].color = sfcolor;
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	line[1].position = sf::Vector2f(p2.x * Pixelize, -(p2.y * Pixelize));
	line[1].color = sfcolor;

	sfcolor = sf::Color::Green;
	line[0].position = sf::Vector2f(p1.x * Pixelize, -(p1.y * Pixelize));
	line[0].color = sfcolor;
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	line[1].position = sf::Vector2f(p2.x * Pixelize, -(p2.y * Pixelize));
	line[1].color = sfcolor;

	m_pRenderTarget->draw(line);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
//	glPointSize(size);
	sf::VertexArray point(sf::Points , 1);
	sf::Color sfcolor((sf::Uint8)(255.0f * color.r), (sf::Uint8)(255.0f * color.g), (sf::Uint8)(255.0f * color.b), 255);
	point[0].position = sf::Vector2f(p.x * Pixelize, -(p.y * Pixelize));
	point[0].color = sfcolor;
	m_pRenderTarget->draw(point);
}

void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
	/*char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);*/
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	sf::Color color((sf::Uint8)(255.0f * c.r), (sf::Uint8)(255.0f * c.g), (sf::Uint8)(255.0f * c.b), 255);
	sf::VertexArray polygonLines(sf::LinesStrip, 4);
	polygonLines[0].position = sf::Vector2f(aabb->lowerBound.x * Pixelize, -(aabb->lowerBound.y * Pixelize));
	polygonLines[0].color = color;

	polygonLines[1].position = sf::Vector2f(aabb->upperBound.x * Pixelize, -(aabb->lowerBound.y * Pixelize));
	polygonLines[1].color = color;
	
	polygonLines[2].position = sf::Vector2f(aabb->upperBound.x * Pixelize, -( aabb->upperBound.y * Pixelize));
	polygonLines[2].color = color;
	
	polygonLines[3].position = sf::Vector2f(aabb->lowerBound.x * Pixelize, -( aabb->upperBound.y * Pixelize));
	polygonLines[3].color = color;
	
	m_pRenderTarget->draw(polygonLines);
}
