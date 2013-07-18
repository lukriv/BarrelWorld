#ifndef __GAME_FLAT_WORLD_CLIENT_IMPL_H__
#define __GAME_FLAT_WORLD_CLIENT_IMPL_H__

#include <wx/vector.h>
#include <wx/scopedptr.h>
#include <wx/thread.h>
#include <sfml/graphics/View.hpp>
#include "../GameSystem/glog.h"
#include "../GameObjects/gobject.h"
#include "../GameObjects/gsceneobj.h"
#include "../GameObjects/ggeometry.h"
#include "../GameObjects/gentityobj.h"
#include "../WorldSrv/gflatworld.h"



typedef wxVector<sf::Texture*> TGameTextureList;
typedef wxVector<IGameGeometry*> TGameGeometryList;
typedef wxVector<GameEntityBase*> TGameEntityList;
typedef wxVector<GameSceneObject*> TSceneGraph;

struct ControlStruct {
public:
	enum StateFlags {
		SF_LEFT = 		(1<<0),
		SF_RIGHT = 		(1<<1),
		SF_UP =			(1<<2),
		SF_DOWN = 		(1<<3),
		SF_MOUSE_LEFT = (1<<4),
		SF_MOUSE_RIGHT =(1<<5)
	};
private:
	wxDword m_state;
	sf::Vector2i m_lastMousePosition;
	sf::Vector2i m_lastMousePositionPressed;
public:
	inline void SetLeft() { m_state |= SF_LEFT; }
	inline void SetRight() { m_state |= SF_RIGHT; }
	inline void SetUp() { m_state |= SF_UP; }
	inline void SetDown() { m_state |= SF_DOWN; }
	inline void SetMouseLeft() { m_state |= SF_MOUSE_LEFT; }
	inline void SetMouseRight() { m_state |= SF_MOUSE_RIGHT; }
	
	inline void RelLeft() { m_state &= ~((wxDword) SF_LEFT); }
	inline void RelRight() { m_state &= ~((wxDword) SF_RIGHT); }
	inline void RelUp() { m_state &= ~((wxDword) SF_UP); }
	inline void RelDown() { m_state &= ~((wxDword) SF_DOWN); }
	inline void RelMouseLeft() { m_state &= ~((wxDword) SF_MOUSE_LEFT); }
	inline void RelMouseRight() { m_state &= ~((wxDword) SF_MOUSE_RIGHT); }
	
	inline bool IsLeftPressed() { return ((m_state & SF_LEFT) != 0); }
	inline bool IsRightPressed() { return ((m_state & SF_RIGHT) != 0); }
	inline bool IsUpPressed() { return ((m_state & SF_UP) != 0); }
	inline bool IsDownPressed() { return ((m_state & SF_DOWN) != 0); }
	inline bool IsMouseLeftPressed() { return ((m_state & SF_MOUSE_LEFT) != 0); }
	inline bool IsMouseRightPressed() { return ((m_state & SF_MOUSE_RIGHT) != 0); }

	inline const sf::Vector2i& GetLastMousePosition() { return m_lastMousePosition; }
	inline void SetLastMousePosition(const sf::Vector2i& mousePos) { m_lastMousePosition = mousePos; }
	
	inline const sf::Vector2i& GetLastMousePositionPressed() { return m_lastMousePositionPressed; }
	inline void SetLastMousePositionPressed(const sf::Vector2i& mousePos) { m_lastMousePositionPressed = mousePos; }
	
};

class GameFlatWorldClient : public IGameFlatWorld {
private:
	GameFlatWorldID m_wrldId;
	GameLoggerPtr m_spLogger;
	
	wxInt32 m_velocityIter;
	wxInt32 m_positionIter;
	float m_timeStep;
	b2Vec2 m_gravity;
	wxScopedPtr<b2World> m_apWorld;
	
	sf::RenderWindow* m_pRenderTarget;
	
	TGameEntityList m_landscape; // all entity list in world - should be divide to characters list, static entities and logic (senzor) entities
	TGameEntityList m_staticObj;
	TGameEntityList m_moveAbleObj;
	TGameEntityList m_characters;
	TGameEntityList m_senzors;
	
	TSceneGraph m_objectMap;
			
	wxCriticalSection m_objectListLock;
	
	ControlStruct m_controls;
	
	bool m_isInitialized;

public: 
	GameFlatWorldClient();
	
	~GameFlatWorldClient();
	
	/*!
	 * \brief Initialize GameFlatWorldClient
	 * \return 
	 */
	GameErrorCode Initialize(sf::RenderWindow *pTarget, GameLogger* pLogger);
	
	/*! \brief Add new entity to world with unique ID
	 * 
	 * \param objID Unique Id (must be unique within this world)
	 * \param pEntity Added entity (cannot be NULL)
	 * \retval FWG_NO_ERROR On success
	 * \retval FWG_E_INVALID_PARAMETER_ERROR if objID is not unique or pEntity is NULL
	 */
	GameErrorCode AddLandscapeEntity( GameEntityBase* pEntity);
	GameErrorCode AddStaticEntity( GameEntityBase* pEntity);
	GameErrorCode AddMoveableEntity( GameEntityBase* pEntity);
	GameErrorCode AddCharacterEntity( GameEntityBase* pEntity);
	GameErrorCode AddSenzorEntity( GameEntityBase* pEntity);
	
	//TODO: Getters for texture, geometry, entities
	/*! \brief Get entity with coresponding ID
	 * \param objId entity id
	 * \return NULL if object with coresponding ID is not found or returns object poiter.
	 */
	GameEntityBase* GetLandscapeEntity( GameObjectId objId);
	GameEntityBase* GetStaticEntity( GameObjectId objId);
	GameEntityBase* GetMoveableEntity( GameObjectId objId);
	GameEntityBase* GetCharacterEntity( GameObjectId objId);
	GameEntityBase* GetSenzorEntity( GameObjectId objId);
	
	inline b2World* GetPhysWorld() { return m_apWorld.get();}
	
	GameErrorCode SetWorldSize(const b2AABB wrldAABB);
	
	
	inline void MoveViewport(const sf::Vector2i& moveVector) 
	{
		sf::View view = m_pRenderTarget->getView();
		view.move(sf::Vector2f(moveVector));
		m_pRenderTarget->setView(view);	
	}
	
	
	void EventProcess(sf::Event &event);
public:
	

	virtual GameFlatWorldID GetFWId();
	
	virtual GameErrorCode SimulationStep();
	virtual GameErrorCode DrawStep();
	virtual GameErrorCode EventStep();
	virtual GameErrorCode AIStep();
	
	virtual GameErrorCode GetUpdateList(GameUpdateStruct** &updList, wxDword &listSize);
};


#endif //__GAME_FLAT_WORLD_CLIENT_IMPL_H__