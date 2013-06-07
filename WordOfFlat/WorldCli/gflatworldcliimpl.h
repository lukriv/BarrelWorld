#ifndef __GAME_FLAT_WORLD_CLIENT_IMPL_H__
#define __GAME_FLAT_WORLD_CLIENT_IMPL_H__

#include "../GameObjects/gobject.h"
#include "../WorldSrv/gflatworld.h"
#include "../GameObjects/gsceneobj.h"

typedef std::map<GameTextureId, sf::Texture*> GameTextureMapType;
typedef std::map<GameShapeId, sf::Drawable*> GameShapeMapType;

class GameFlatWorldClient : public IGameFlatWorld {
private:
	GameFlatWorldID m_wrldId;
	GameLoggerPtr m_spLogger;
	
	b2Vec2 m_gravity;
	wxScopedPtr<b2World> m_apWorld;
	
	wxVector<IGameObject*> m_objectsVec; //!< all world objects (every object must be deleted at the end)

	GameTextureMapType m_textureMap;
	GameShapeMapType m_dynamicShapeMap;
	wxVector<GameSceneObject*> m_objectMap;
		
	bool m_isInitialized;

public: 
	GameFlatWorldClient() {}
	~GameFlatWorldClient();
	
	/*! \brief Add drawable object to flat world client
	 * \param pObject added object
	 * \param objectId returned ID
	 * \retval FWG_NO_ERROR on success
	 * \retval error on failed
	 */
	GameErrorCode AddDrawableObject( GameObjectId objId, IGameObject* pObject);
	GameErrorCode AddTexture( GameTextureId texId, sf::Texture* pTexture);
	GameErrorCode AddShape( GameShapeId shapeId, sf::Drawable* pShape);
	
	/*! \brief Create new object within this world
	 * 
	 * This object is not initialized and must be added in this world with unique ID.
	 * \param bodyDef Body definition (Box2d)
	 * \param object Created object
	 * \retval FWG_NO_ERROR On success
	 * \retval Other errorcode on fail
	 * \warning Object created within this world cannot be used in other GameFlatWorldSrv!
	 */
	GameErrorCode CreateNewObject(b2BodyDef &bodyDef, IGameObjectSrv *&object);
	
	/*! \brief Add new object to world with unique ID
	 * 
	 * \param objID Unique Id (must be unique within this world)
	 * \param object Added object (cannot be NULL)
	 * \retval FWG_NO_ERROR On success
	 * \retval FWG_E_INVALID_PARAMETER_ERROR if objID is not unique or pObject is NULL
	 */
	GameErrorCode AddNewObject(GameObjectId objID, IGameObjectSrv *pObject);
	
	GameErrorCode SetWorldSize(const b2Vec2 &LLpoint, const b2Vec2 &RUpoint);
	
	GameErrorCode LoadWorld(const wxChar* worldName);
public:
	virtual GameFlatWorldID GetFWId() = 0;
	
	virtual GameErrorCode SimulationStep() = 0;
	virtual GameErrorCode DrawStep() = 0;
	virtual GameErrorCode EventStep() = 0;
	virtual GameErrorCode AIStep() = 0;
	
	virtual GameErrorCode GetUpdateList(GameUpdateStruct** &updList, wxDword &listSize) = 0;
	
}


#endif //__GAME_FLAT_WORLD_CLIENT_IMPL_H__