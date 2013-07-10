#ifndef __GAME_FLAT_WORLD_CLIENT_IMPL_H__
#define __GAME_FLAT_WORLD_CLIENT_IMPL_H__

#include <wx/vector.h>
#include "../GameObjects/gobject.h"
#include "../WorldSrv/gflatworld.h"
#include "../GameObjects/gsceneobj.h"

typedef wxVector<sf::Texture*> TGameTextureList;
typedef wxVector<IGameGeometry*> TGameGeometryList;
typedef wxVector<GameEntityBase*> TGameEntityList;
typedef wxVector<GameSceneObject*> TSceneGraph;



class GameFlatWorldClient : public IGameFlatWorld {
private:
	GameFlatWorldID m_wrldId;
	GameLoggerPtr m_spLogger;
	
	wxInt32 m_velocityIter;
	wxInt32 m_positionIter;
	float m_timeStep;
	b2Vec2 m_gravity;
	wxScopedPtr<b2World> m_apWorld;
	
	sf::RenderTarget* m_pRenderTarget;
	
	TGameEntityList m_landscape; // all entity list in world - should be divide to characters list, static entities and logic (senzor) entities
	TGameEntityList m_staticObj;
	TGameEntityList m_moveAbleObj;
	TGameEntityList m_characters;
	TGameEntityList m_senzors;
	
	TSceneGraph m_objectMap;
			
	wxCriticalSection m_objectListLock;
	
	bool m_isInitialized;

public: 
	GameFlatWorldClient();
	
	~GameFlatWorldClient();
	
	/*!
	 * \brief Initialize GameFlatWorldClient
	 * \return 
	 */
	GameErrorCode Initialize(sf::RenderTarget *pTarget, GameLogger* pLogger);
	
	/*! \brief Add new entity to world with unique ID
	 * 
	 * \param objID Unique Id (must be unique within this world)
	 * \param pEntity Added entity (cannot be NULL)
	 * \retval FWG_NO_ERROR On success
	 * \retval FWG_E_INVALID_PARAMETER_ERROR if objID is not unique or pEntity is NULL
	 */
	GameErrorCode AddLandscapeEntity( GameObjectId objId, GameEntityBase* pEntity);
	GameErrorCode AddStaticEntity( GameObjectId objId, GameEntityBase* pEntity);
	GameErrorCode AddMoveableEntity( GameObjectId objId, GameEntityBase* pEntity);
	GameErrorCode AddCharacterEntity( GameObjectId objId, GameEntityBase* pEntity);
	GameErrorCode AddSenzorEntity( GameObjectId objId, GameEntityBase* pEntity);
	
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
public:


	virtual GameFlatWorldID GetFWId();
	
	virtual GameErrorCode SimulationStep();
	virtual GameErrorCode DrawStep();
	virtual GameErrorCode EventStep();
	virtual GameErrorCode AIStep();
	
	virtual GameErrorCode GetUpdateList(GameUpdateStruct** &updList, wxDword &listSize);
}


#endif //__GAME_FLAT_WORLD_CLIENT_IMPL_H__