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
	
	b2Vec2 m_gravity;
	wxScopedPtr<b2World> m_apWorld;
	
	TGameTextureList m_textureMap;
	TGameGeometryList m_geometryMap;
	TGameEntityList m_entitiesList; // all entity list in world - should be divide to characters list, static entities and logic (senzor) entities
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
	GameErrorCode Initialize(GameLogger *pLogger);
	
	/*! \brief Add new entity to world with unique ID
	 * 
	 * \param objID Unique Id (must be unique within this world)
	 * \param pEntity Added entity (cannot be NULL)
	 * \retval FWG_NO_ERROR On success
	 * \retval FWG_E_INVALID_PARAMETER_ERROR if objID is not unique or pEntity is NULL
	 */
	GameErrorCode AddEntity( GameObjectId objId, GameEntityBase* pEntity);
	GameErrorCode AddTexture( GameTextureId texId, sf::Texture* pTexture);
	GameErrorCode AddGeometry( GameShapeId shapeId, IGameGeometry* pShape);
	
	//TODO: Getters for texture, geometry, entities
	/*! \brief Get entity with coresponding ID
	 * \param objId entity id
	 * \return NULL if object with coresponding ID is not found or returns object poiter.
	 */
	GameEntityBase* GetEntity( GameObjectId objId);
	sf::Texture* GetTexture( GameTextureId texId);
	IGameGeometry* GetGeometry( GameShapeId shapeId);
	
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