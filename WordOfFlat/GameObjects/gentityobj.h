#ifndef __GAME_ENTITY_OBJECT_H__
#define __GAME_ENTITY_OBJECT_H__


#include <SFML/Graphics.hpp>
#include <wx/vector.h>
#include "../GameSystem/glog.h"
#include "gsceneobj.h"
#include "ggeometry.h"
/*!
 * \class GameEntityBase
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Entity base class
 */
class GameEntityBase : public GameSceneObject, public sf::Transformable {
	GameObjStateStruct m_state;
private:
	GameObjectType m_enType;
public:
	GameEntityBase(GameObjectType enType) : m_enType(enType){}
	
	GameObjStateStruct& GetObjectState() { return m_state;}
	GameObjectType GetType() {return m_enType;}
	
	virtual void UpdatePosition() = 0;
	virtual void TraceLogInfo(GameLogger *pLogger) = 0;
};


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
public:
	GameEntity(GameObjectType enType) : 
				GameEntityBase(enType),
				m_pGeometry(NULL),
				m_pTexture(NULL),
				m_pBody(NULL) {}
	
	inline void SetGeometry (IGameGeometry *pGeometry) { m_pGeometry = pGeometry;}
	inline void SetTexture (sf::Texture *pTexture) {m_pTexture = pTexture;}
	inline void SetBody (b2Body *pBody) {m_pBody = pBody;}
	
	inline IGameGeometry* GetGeometry () { return m_pGeometry;}
	inline sf::Texture* GetTexture () { return m_pTexture;}
	inline b2Body* GetBody () { return m_pBody;}
	
public:
	virtual void UpdatePosition();
	virtual void TraceLogInfo(GameLogger *pLogger);
	
public:	
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const;
	
};


/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class GameEntityGroup : public GameEntityBase {
	wxVector<GameEntity*> m_entityGroup;	
public:
	GameEntityGroup() : GameEntityBase(GAME_OBJECT_TYPE_ENTITY_GROUP) {}
	
	void PushBackEntity (GameEntity* pEntity) {
		m_entityGroup.push_back(pEntity);
	}

public:
	virtual void UpdatePosition();
	virtual void TraceLogInfo(GameLogger *pLogger);
	
public:	
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const;
	
};


#endif //__GAME_ENTITY_OBJECT_H__