#ifndef __GAME_ENTITY_OBJECT_H__
#define __GAME_ENTITY_OBJECT_H__


#include <SFML/Graphics.hpp>
#include <wx/vector.h>
#include "../GameSystem/glog.h"
#include "gtransformable.h"
#include "gsceneobj.h"
#include "ggeometry.h"

enum GameEntityReason {
	ENTITY_REASON_UNDEFINED = 0,
	ENTITY_REASON_DIED 		= 1
};

struct GameEntityUpdateStruct {
	sf::Time m_timeDiff;
};

//forward declaration
class GameEntityBase;

class GameEntityCallback {
public:
	void ChangeEntityStatus(const GameEntityReason &reason, const GameEntityBase *pEntity);
	
	sf::RenderTexture& GetRenderTexture();
	sf::RenderTexture& GetRenderTexture(const sf::Vector2f& neededSize);
};

/*!
 * \class GameEntityBase
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Entity base class
 */
class GameEntityBase : public GameSceneObject, public GameTransformable {
	GameObjStateStruct m_state;
private:
	GameObjectType m_enType;
	GameEntityCallback *m_pEntityCallback;
public:
	GameEntityBase(GameObjectType enType) : m_enType(enType), m_pEntityCallback(NULL) {}
	virtual ~GameEntityBase() {}
	
	inline void SetCallback(GameEntityCallback *pEntityCallback) {m_pEntityCallback = pEntityCallback;}
	inline GameEntityCallback* GetCallback() { return m_pEntityCallback; }
	
	GameObjStateStruct& GetObjectState() { return m_state;}
	GameObjectType GetType() {return m_enType;}
	
	virtual bool IsEntity() { return true; }

	
public:
	virtual void UpdateEntity(const GameEntityUpdateStruct& updStruct) = 0;
	virtual void TraceLogInfo(GameLogger *pLogger) = 0;
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
	virtual void UpdateEntity(const GameEntityUpdateStruct& updStruct);
	virtual void TraceLogInfo(GameLogger *pLogger);
	
public:	
	virtual void draw( sf::RenderTarget& target, sf::RenderStates states) const;
	
};


#endif //__GAME_ENTITY_OBJECT_H__