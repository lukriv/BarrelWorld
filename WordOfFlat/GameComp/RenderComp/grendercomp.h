#ifndef __GAME_RENDER_COMPONENT_H__
#define __GAME_RENDER_COMPONENT_H__

#include "gtransformable.h"

/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class GameEntity : public sf::Drawable, public GameTransformable {
	IGameGeometry *m_pGeometry;
	sf::Texture *m_pTexture; //!< Actual texture which will be displayed in the next display loop iteration
	sf::Texture *m_pBaseTexture; //!< Base texture on which will be added the modifications
	GameObjectID m_animID;
	sf::Texture m_internalTexture;
	sf::BlendMode m_blendmode;
public:
	GameEntity() : GameEntityBase(GAME_OBJECT_TYPE_BASIC_ENTITY),
				m_pGeometry(NULL),
				m_pTexture(NULL),
				m_pBaseTexture(NULL),
				m_animID(GAME_OBJECT_ID_INVALID),
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
	inline void SetAnimation (GameObjectID animID) {m_animID = animID;}
	inline void SetBlendMode(sf::BlendMode blendmode) {m_blendmode = blendmode;}
	
	inline IGameGeometry* GetGeometry () { return m_pGeometry;}
	inline sf::Texture* GetTexture () { return m_pTexture;}
	inline GameObjectID GetAnimation() { return m_animID; }
	
public:
	virtual void DebugInfo (GameLogger* pLogger);
	
public:	
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const;
	
};


#endif //__GAME_RENDER_COMPONENT_H__