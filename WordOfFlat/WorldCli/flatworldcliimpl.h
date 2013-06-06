#ifndef __GAME_FLAT_WORLD_CLIENT_IMPL_H__
#define __GAME_FLAT_WORLD_CLIENT_IMPL_H__

#include "../GameObjectsSrv/gameupdobj.h"
#include "flatworldcli.h"

typedef std::map<GameTextureId, sf::Texture*> GameTextureMapType;
typedef std::map<GameShapeId, sf::Drawable*> GameShapeMapType;
typedef std::map<GameObjectId, IGameObject*> GameObjectMapType;

class GameFlatWorldClient : public IGameFlatWorldClient {
private:
	GameTextureMapType m_textureMap;
	GameShapeMapType m_dynamicShapeMap;
	GameObjectMapType m_objectMap;

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
	
	/*! \brief Get texture for corespond ID
	 * \param texId Texture ID
	 * \return Pointer to texture on success, NULL if texture will not found
	 */
	sf::Texture* GetTexture(GameTextureId texId);
	
	/*! \brief Get shape for corespond ID
	 * \param shapeId Shape ID
	 * \return Pointer to shape on success, NULL if shape will not found
	 */
	sf::Shape* GetShape(GameShapeId shapeId);
	
	/*! \brief Get object for correspond ID
	 * \param objId Object ID
	 * \return Pointer to object on success, NULL if object will not found
	 */
	IGameObject* GetObject(GameObjectId objId);
public:
	virtual bool DrawScreen();
	
}


#endif //__GAME_FLAT_WORLD_CLIENT_IMPL_H__