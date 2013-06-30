#ifndef __GAME_RESOURCE_HOLDER_H__
#define __GAME_RESOURCE_HOLDER_H__

#include "../GameSystem/glog.h"

#include "gresloader.h"




class GameResourceHolder : public IRefObject {
private:
	static GameResourceHolder* g_pResourceHolder;
private:
	wxAtomicInt m_refCount;
	GameLoggerPtr m_spLogger;
	IGameResourceLoader *m_pResLoader;
	
	TGameTextureMap m_texMap;
	TGameGeometryMap m_geomMap;
	
	wxCriticalSection m_resouceLocker;
	
	bool m_isInitialized; 
private:
	GameResourceHolder(): m_refCount(1), m_pResLoader(NULL), m_isInitialized(false){}
	~GameResourceHolder();
	
	GameErrorCode Initialize(GameLogger *pLogger, IGameResourceLoader *pLoader);
	
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
	 * \param[in] texID wanted texture
	 * \return Pointer to texture or NULL if texture was not found
	 */
	sf::Texture* GetTexture(GameTextureId texID);
	
	/*! \brief Get geometry object with given parameter
	 * 
	 * Increment internal reference counter for given the geometry object
	 * 
	 * \param[in] geomID wanted geometry object
	 * \return Pointer to geometry object or NULL if geometry object was not found
	 */
	IGameGeometry* GetGeometry(GameShapeId geomID);
	
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