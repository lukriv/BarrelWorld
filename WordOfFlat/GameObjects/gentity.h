#ifndef __GAME_ANIMATED_ENTITY_H__
#define __GAME_ANIMATED_ENTITY_H__

#include "gentityobj.h"
#include "ganimation.h"

/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class GameEntity : public GameEntityBase {
	IGameGeometry *m_pGeometry;
	sf::Texture *m_pTexture; //!< Actual texture which will be displayed in the next display loop iteration
	sf::Texture *m_pBaseTexture; //!< Base texture on which will be added the modifications
	b2Body *m_pBody;
	GameAnimation *m_pAnimation;
	sf::Texture m_internalTexture;
	sf::BlendMode m_blendmode;
public:
	GameEntity() : GameEntityBase(GAME_OBJECT_TYPE_BASIC_ENTITY),
				m_pGeometry(NULL),
				m_pTexture(NULL),
				m_pBaseTexture(NULL),
				m_pBody(NULL),
				m_pAnimation(NULL),
				m_blendmode(sf::BlendNone){}
	
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
		bool result = m_internalTexture.loadFromImage(image);
		m_pTexture = &m_internalTexture;
		return result;
	}
	
	inline void SetBaseTexture(sf::Texture *pTexture) 
	{
		m_pBaseTexture = pTexture;
		if (pTexture != NULL) 
		{
			m_internalTexture = *pTexture;
		}
		m_pTexture = &m_internalTexture;
	}
	
	
	inline void SetGeometry (IGameGeometry *pGeometry) { m_pGeometry = pGeometry;}
	inline void SetTexture (sf::Texture *pTexture) {m_pTexture = pTexture;}
	inline void SetBody (b2Body *pBody) {m_pBody = pBody;}
	inline void SetAnimation (GameAnimation *pAnimation) {m_pAnimation = pAnimation;}
	inline void SetBlendMode(sf::BlendMode blendmode) {m_blendmode = blendmode;}
	
	inline IGameGeometry* GetGeometry () { return m_pGeometry;}
	inline sf::Texture* GetTexture () { return m_pTexture;}
	inline b2Body* GetBody () { return m_pBody;}
	inline GameAnimation* GetAnimation() { return m_pAnimation; }
	
public:
	virtual void UpdateEntity(const GameEntityUpdateStruct& updStruct);
	virtual void TraceLogInfo(GameLogger *pLogger);
	virtual void DebugInfo (GameLogger* pLogger);
	
public:	
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const;
	
};


#endif //__GAME_ANIMATED_ENTITY_H__