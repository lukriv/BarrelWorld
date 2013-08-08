#ifndef __GAME_ANIMATED_ENTITY_H__
#define __GAME_ANIMATED_ENTITY_H__

#include "gentityobj.h"


/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class GameEntity : public GameEntityBase {
	IGameGeometry *m_pGeometry;
	sf::Texture *m_pTexture;
	b2Body *m_pBody;
	GameAnimation *m_pAnimation;
	sf::Texture m_internalTexture;
public:
	GameEntity(GameObjectType enType) : 
				GameEntityBase(enType),
				m_pGeometry(NULL),
				m_pTexture(NULL),
				m_pBody(NULL),
				m_pAnimation(NULL){}
	
	~GameEntity() 
	{
		if(m_pTexture != NULL)
		{
			delete m_pTexture;
			m_pTexture = NULL;
		}
	}
	
	inline bool CreateTexture (const sf::Image& image)
	{
		m_internalTexture.loadFromImage(image);
		m_pTexture = &m_internalTexture;
	}
	
	inline void CopyTexture(const sf::Texture& texture) 
	{
		m_internalTexture = texture; 
		m_pTexture = &m_internalTexture;
	}
	
	
	inline void SetGeometry (IGameGeometry *pGeometry) { m_pGeometry = pGeometry;}
	inline void SetTexture (sf::Texture *pTexture) {m_pTexture = pTexture;}
	inline void SetBody (b2Body *pBody) {m_pBody = pBody;}
	inline void SetAnimation (GameAnimation *pAnimation);
	
	inline IGameGeometry* GetGeometry () { return m_pGeometry;}
	inline sf::Texture* GetTexture () { return m_pTexture;}
	inline b2Body* GetBody () { return m_pBody;}
	inline GameAnimation* GetAnimation() { return m_pAnimation; }
	
public:
	virtual void UpdateEntity(const GameEntityUpdateStruct& updStruct);
	virtual void TraceLogInfo(GameLogger *pLogger);
	
public:	
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const;
	
};


#endif //__GAME_ANIMATED_ENTITY_H__