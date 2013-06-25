#ifndef __GAME_RESOURCE_HOLDER_H__
#define __GAME_RESOURCE_HOLDER_H__

#include "../GameSystem/glog.h"

#include "gresloader.h"




class GameResourceHolder {
private:
	static GameResourceHolder* g_pResourceHolder;
private:
	bool m_isInitialized;
	GameLoggerPtr m_spLogger;
	 
private:
	GameResourceHolder(): m_isInitialized(false){}
	~GameResourceHolder();
	
	GameErrorCode Initialize(GameLogger *pLogger);
	
public:
	GameErrorCode SetLoader(IGameResourceLoader *pLoader);
	
	sf::Texture* GetTexture(GameTextureId texID);
	IGameGeometry* GetGeometry(GameShapeId geomID);

	
public:
	/*! \brief Initialize resource holder singleton
	 * \param pLogger logger
	 * \retval FWG_NO_ERROR on success
	 * \retval Some errorcode on failture
	 */
	static GameErrorCode InitializeResourceHolder(GameLogger *pLogger = NULL);
	
	static GameResourceHolder* GetResourceHolder();
	
};


#endif //__GAME_RESOURCE_HOLDER_H__