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
#include "../GameObjects/gentity.h"
#include "../GameObjects/ganimation.h"
#include "../WorldSrv/gflatworld.h"
#include "../GameResHold/gentityfactory.h"

#include "debugrender.h"



typedef wxVector<sf::Texture*> TGameTextureList;
typedef wxVector<IGameGeometry*> TGameGeometryList;
typedef wxVector<GameEntityBase*> TGameEntityBasePtrList;
typedef wxVector<GameEntity*> TGameEntityPtrList;
typedef wxVector<GameAnimation*> TGameAnimPtrList;
typedef wxVector<GameSceneObject*> TSceneGraph;

class ControlStruct {
public:
	enum StateFlags {
		SF_LEFT = 				(1<<0),
		SF_RIGHT = 				(1<<1),
		SF_UP =					(1<<2),
		SF_DOWN = 				(1<<3),
		SF_MOUSE_LEFT = 		(1<<4),
		SF_MOUSE_RIGHT =		(1<<5),
		SF_MOUSE_WHEEL_MOVE =	(1<<6)
	};
private:
	wxDword m_state; //control state vector
	wxDword m_processed; //note thate state was processed yet - reseted by release
	sf::Vector2i m_lastMousePosition;
	sf::Vector2i m_lastMousePositionPressed;
	wxInt32 m_mouseWheelDelta;
public:
	inline void SetLeft() { m_state |= SF_LEFT; }
	inline void SetRight() { m_state |= SF_RIGHT; }
	inline void SetUp() { m_state |= SF_UP; }
	inline void SetDown() { m_state |= SF_DOWN; }
	inline void SetMouseLeft() { m_state |= SF_MOUSE_LEFT; }
	inline void SetMouseRight() { m_state |= SF_MOUSE_RIGHT; }
	inline void SetMouseWheel() { m_state |= SF_MOUSE_WHEEL_MOVE; }
	
	inline void RelLeft() { m_state &= ~((wxDword) SF_LEFT); m_processed &= ~((wxDword) SF_LEFT);}
	inline void RelRight() { m_state &= ~((wxDword) SF_RIGHT); m_processed &= ~((wxDword) SF_RIGHT);}
	inline void RelUp() { m_state &= ~((wxDword) SF_UP); m_processed &= ~((wxDword) SF_UP); }
	inline void RelDown() { m_state &= ~((wxDword) SF_DOWN); m_processed &= ~((wxDword) SF_DOWN);  }
	inline void RelMouseLeft() { m_state &= ~((wxDword) SF_MOUSE_LEFT); m_processed &= ~((wxDword) SF_MOUSE_LEFT);}
	inline void RelMouseRight() { m_state &= ~((wxDword) SF_MOUSE_RIGHT); m_processed &= ~((wxDword) SF_MOUSE_RIGHT);}
	inline void RelMouseWheel() { m_state &= ~((wxDword) SF_MOUSE_WHEEL_MOVE); m_processed &= ~((wxDword) SF_MOUSE_WHEEL_MOVE);}
	
	inline bool IsLeftPressed() { return ((m_state & SF_LEFT) != 0); }
	inline bool IsRightPressed() { return ((m_state & SF_RIGHT) != 0); }
	inline bool IsUpPressed() { return ((m_state & SF_UP) != 0); }
	inline bool IsDownPressed() { return ((m_state & SF_DOWN) != 0); }
	inline bool IsMouseLeftPressed() { return ((m_state & SF_MOUSE_LEFT) != 0); }
	inline bool IsMouseRightPressed() { return ((m_state & SF_MOUSE_RIGHT) != 0); }
	inline bool IsMouseWheelMove() { return ((m_state & SF_MOUSE_WHEEL_MOVE) != 0); }
	
	inline void SetLeftProcessed() { m_processed |= SF_LEFT; }
	inline void SetRightProcessed() { m_processed |= SF_RIGHT; }
	inline void SetUpProcessed() { m_processed |= SF_UP; }
	inline void SetDownProcessed() { m_processed |= SF_DOWN; }
	inline void SetMouseLeftProcessed() { m_processed |= SF_MOUSE_LEFT; }
	inline void SetMouseRightProcessed() { m_processed |= SF_MOUSE_RIGHT; }
	inline void SetMouseWheelProcessed() { m_processed |= SF_MOUSE_WHEEL_MOVE; }
	
	inline bool WasLeftProcessed() { return ((m_processed & SF_LEFT) != 0); }
	inline bool WasRightProcessed() { return ((m_processed & SF_RIGHT) != 0); }
	inline bool WasUpProcessed() { return ((m_processed & SF_UP) != 0); }
	inline bool WasDownProcessed() { return ((m_processed & SF_DOWN) != 0); }
	inline bool WasMouseLeftProcessed() { return ((m_processed & SF_MOUSE_LEFT) != 0); }
	inline bool WasMouseRightProcessed() { return ((m_processed & SF_MOUSE_RIGHT) != 0); }
	inline bool WasMouseWheelMove() { return ((m_processed & SF_MOUSE_WHEEL_MOVE) != 0); }

	inline const sf::Vector2i& GetLastMousePosition() { return m_lastMousePosition; }
	inline void SetLastMousePosition(const sf::Vector2i& mousePos) { m_lastMousePosition = mousePos; }
	
	inline const sf::Vector2i& GetLastMousePositionPressed() { return m_lastMousePositionPressed; }
	inline void SetLastMousePositionPressed(const sf::Vector2i& mousePos) { m_lastMousePositionPressed = mousePos; }
	
	inline const wxInt32 GetMouseWheelDelta() {return m_mouseWheelDelta;}
	inline void SetMouseWheelDelta(wxInt32 mouseWheelDelta) { m_mouseWheelDelta = mouseWheelDelta;}
	
};


struct DisplayState{
	float actualZoom;

public:
	DisplayState() : actualZoom(1.0f) {}
};


class GameFlatWorldClient : public IGameFlatWorld, public GameEntityCallback {
private:
	GameFlatWorldID m_wrldId;
	GameLoggerPtr m_spLogger;
	
	wxInt32 m_velocityIter;
	wxInt32 m_positionIter;
	float m_timeStep;
	b2Vec2 m_gravity;
	wxScopedPtr<b2World> m_apWorld;
	
	sf::RenderWindow* m_pRenderTarget;
	RefObjSmPtr<GameEntityFactory> m_spEntityFactory;
	
	TGameEntityBasePtrList m_landscape; // all entity list in world - should be divide to characters list, static entities and logic (senzor) entities
	TGameEntityBasePtrList m_staticObj;
	TGameEntityBasePtrList m_moveAbleObj;
	TGameEntityBasePtrList m_characters;
	TGameEntityBasePtrList m_senzors;
	
	TGameEntityPtrList m_freeEntities;
	TGameAnimPtrList m_freeAnimations;
	
	TSceneGraph m_objectMap;
			
	wxCriticalSection m_objectListLock;
	
	ControlStruct m_controls;
	
	DisplayState m_dispState;
	
	DebugDraw *m_pB2DebugDraw;
	
	sf::RenderTexture m_renderTexture256;
	sf::RenderTexture m_renderTexture512;
	sf::RenderTexture m_renderTexture1024;
	
	bool m_isInitialized;

public: 
	GameFlatWorldClient();
	
	~GameFlatWorldClient();
	
	/*!
	 * \brief Initialize GameFlatWorldClient
	 * \return 
	 */
	GameErrorCode Initialize(sf::RenderWindow *pTarget, GameEntityFactory *pEntFactory, GameLogger* pLogger);
	
	
	/*!
	 * \brief CreateNew* gets new entity
	 * This can return new entity or not used instance (life of item ends)
	 * 
	 * \return FWG_NO_ERROR on succes, errorcode otherwise
	 */
	GameErrorCode CreateNewEntity(GameEntity *&pEntity);
	GameErrorCode CreateNewAnimation(GameAnimation *&pAnim);
	
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
	GameEntityBase* GetLandscapeEntity( GameObjectID objId);
	GameEntityBase* GetStaticEntity( GameObjectID objId);
	GameEntityBase* GetMoveableEntity( GameObjectID objId);
	GameEntityBase* GetCharacterEntity( GameObjectID objId);
	GameEntityBase* GetSenzorEntity( GameObjectID objId);
	
	inline b2World* GetPhysWorld() { return m_apWorld.get();}
	
	GameErrorCode SetWorldSize(const b2AABB wrldAABB);
	
	
	inline void MoveViewport(const sf::Vector2i& moveVector) 
	{
		sf::View view = m_pRenderTarget->getView();
		view.move(sf::Vector2f(moveVector) * m_dispState.actualZoom);
		m_pRenderTarget->setView(view);	
	}
	
	inline void ZoomViewport( float zoomFactor )
	{
		sf::View view = m_pRenderTarget->getView();
		view.zoom( zoomFactor );
		m_pRenderTarget->setView(view);	
	}
	
	void EventProcess(sf::Event &event);
	
	void EnableDebugDraw();
public:
	

	virtual GameFlatWorldID GetFWId();
	
	virtual GameErrorCode SimulationStep();
	virtual GameErrorCode DrawStep();
	virtual GameErrorCode EventStep();
	virtual GameErrorCode AIStep();
	
	virtual GameErrorCode GetUpdateList(GameUpdateStruct** &updList, wxDword &listSize);
public:
	void ChangeEntityStatus(const GameEntityReason &reason, const GameEntityBase *pEntity);
	
	sf::RenderTexture& GetRenderTexture();
	sf::RenderTexture& GetRenderTexture(const sf::Vector2f& neededSize);
};


#endif //__GAME_FLAT_WORLD_CLIENT_IMPL_H__