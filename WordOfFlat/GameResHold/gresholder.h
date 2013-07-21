#ifndef __GAME_RESOURCE_HOLDER_H__
#define __GAME_RESOURCE_HOLDER_H__

#include <sfml/system/Mutex.hpp>
#include "../GameSystem/glog.h"


#include "gresloader.h"




class GameResourceHolder : public IRefObject {
private:
	static GameResourceHolder* g_pResourceHolder;
private:
	wxCriticalSection m_resouceLocker;
	wxAtomicInt m_refCount;
	GameLoggerPtr m_spLogger;
	IGameResourceLoader *m_pResLoader;
	
	TGameTextureMap m_texMap;
	TGameGeometryMap m_geomMap;
	TGamePhysJointMap m_physJointMap;
	TGamePhysBodyMap m_physBodyMap;
	TGamePhysFixMap m_physFixMap;
		
	bool m_isInitialized; 
private:
	GameResourceHolder(): m_refCount(1),
				m_pResLoader(NULL),
				m_isInitialized(false){}
	~GameResourceHolder();
	
	GameErrorCode Initialize(GameLogger *pLogger, IGameResourceLoader *pLoader);
	
	/*! \brief Load initial values to the maps
	 * \retval FWG_NO_ERROR on success
	 * \retval Error code on fail
	 */
	GameErrorCode LoadResourceMaps();
	
	/*! \brief Clear and release all maps and resources */
	void ClearResourceMaps();
	
public:
	/*! \brief Set right resource holder
	 * \param[in] pLoader Poiter to resource holder (cannot be NULL)
	 * \retval FWG_NO_ERROR on success
	 * \retval FWG_E_INVALID_PARAMETER_ERROR if pLoader is NULL
	 */
	GameErrorCode SetLoader(IGameResourceLoader *pLoader);
	
	/*! \brief Get texture with given parameter
	 * 
	 * Increment internal reference counter for given the texture
	 * 
	 * \param[in] texID wanted texture image
	 * \return Pointer to texture image or NULL if texture image was not found
	 */
	sf::Image* GetTexture(GameTextureId texID);
	
	/*! \brief Get geometry object with given parameter
	 * 
	 * Increment internal reference counter for given the geometry object
	 * 
	 * \param[in] geomID wanted geometry object
	 * \return Pointer to geometry object or NULL if geometry object was not found
	 */
	GameGeometryContainer* GetGeometry(GameShapeId geomID);
	
	/*! \brief Get joint definition
	 * \param[in] jointID Joint identificator
	 * \param[out] pJointDef Joint definition
	 * \param[out] bodyList Body list for joint
	 * \retval FWG_NO_ERROR on success
	 * \retval Error code on failture	
	 */
	GameErrorCode GetJointDef(GamePhysObjId jointID, b2JointDef *&pJointDef, wxVector<GamePhysObjId> &bodyList);
	
	/*! \brief Get body definition
	 * \param[in] bodyID Body identificator
	 * \param[out] pBodyDef Body definition 
	 * \param[out] fixtureList Fixture reference list
	 * \retval FWG_NO_ERROR on success
	 * \retval Error code on failture	
	 */
	GameErrorCode GetBodyDef(GamePhysObjId bodyID, b2BodyDef *&pBodyDef, wxVector<GamePhysObjId> &fixtureList);
	
	/*! \brief Get fixture definition
	 * \param[in] fixID
	 * \param[out] pFixtureDef
	 * \param[out] shapeID
	 * \retval FWG_NO_ERROR on success
	 * \retval Error code on failture
	 */
	GameErrorCode GetFixtureDef(GamePhysObjId fixID, b2FixtureDef *&pFixtureDef, GameShapeId &shapeID);
	
	/*! \brief Release texture
	 * 
	 * Decrement reference count for the texture.
	 * 
	 * \param texID Released texture
	 */
	void ReleaseTexture(GameTextureId texID);
	
	/*! \brief Release geometry object
	 * 
	 * Decrement reference count for the geometry object.
	 * 
	 * \param geomID Released geometry object
	 */
	void ReleaseGeometry(GameShapeId geomID);
	
	/*! \brief Release joint
	 * 
	 * Decrement reference count for the joint.
	 * 
	 * \param jointID Released joint
	 */
	void ReleaseJoint(GamePhysObjId jointID);
	
	/*! \brief Release body
	 * 
	 * Decrement reference count for the body.
	 * 
	 * \param bodyID Released body
	 */
	void ReleaseBody(GamePhysObjId bodyID);
	
	/*! \brief Release fixture
	 * 
	 * Decrement reference count for the fixture.
	 * 
	 * \param fixID Released fixture
	 */
	void ReleaseFixture(GamePhysObjId fixID);

public:
	virtual void addRef();
	virtual wxInt32 release();
	
public:
	/*! \brief Initialize resource holder singleton
	 * \param pLogger logger
	 * \retval FWG_NO_ERROR on success
	 * \retval Some errorcode on failture
	 */
	static GameErrorCode InitializeResourceHolder(GameLogger *pLogger = NULL);
	
	/*! \brief Release resource holder
	 * 
	 * Releases one refcount from resource holder and set g_pResourceHolder to NULL. 
	 * Physically is Resource Holder freed when reference count reach the zero.
	 */
	static void ReleaseResourceHolder();
	
	/*! \brief Returns pointer to resource holder
	 * 
	 * If method is called before InitializeResourceHolder or after ReleaseResourceHolder, it returns NULL.
	 * 
	 * \return Pointer to resource holder
	 */
	static GameResourceHolder* GetResourceHolder();
	
};


#endif //__GAME_RESOURCE_HOLDER_H__