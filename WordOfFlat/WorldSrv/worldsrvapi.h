#ifndef __GAME_WORLD_SERVER_API_H__
#define __GAME_WORLD_SERVER_API_H__



/*!
 * \class IWorldSrvApi
 * \author Lukas
 * \date 30.1.2013
 * \file worldsrvapi.h
 * \brief World server api
 * 
 *  Api for creating and storing GlobalWorlds. It is supposed that this class will be
 *		a singleton in game server application.
 */
class IWorldSrvApi {
	
	/*! 
	 * \brief Generates new Global World Server.
	 * 
	 * This is to create testing world.
	 * 
	 * \param WorldName Identification of Global World (one word name is highly recomended) - must be unique
	 * \param pGlobWldSrv Newly created Global World Server.
	 * \retval FWG_NO_ERROR on success
	 * \retval error code 
	 */
	virtual GameErrorCode GenerateGlobalWorld(const wxChar* WorldName, IGlobalWorldSrv &* pGlobWldSrv) = 0;
	
	/*!
	 * \brief Load Global World Server from file.
	 * 
	 * \param FileName Name of file with global world data 
	 * \param WorldName Identification of Global World (one word name is highly recomended) - must be unique
	 * \param pGlobWldSrv Newly created Global World Server.
	 * \retval FWG_NO_ERROR on success
	 * \retval error code 
	 */
	virtual GameErrorCode LoadGlobalWorld(const wxChar* FileName, const wxChar* WorldName, IGlobalWorldSrv * pGlobWldSrv) = 0;
	
	/*!
	 * \brief Get existed global world server.
	 * 
	 * If Global World Server with given name does not exist, it returns error and Null pointer.
	 * 
	 * \param WorldName Name of searched GWS
	 * \param pGlobWldSrv Global World Server (returns NULL if GWS does not exist)
	 * \retval FWG_NO_ERROR on success
	 * \retval error code 
	 */
	virtual GameErrorCode GetGlobalWorld(const wxChar* WorldName, IGlobalWorldSrv &* pGlobWldSrv) = 0;
		
}

#endif //__GAME_WORLD_SERVER_API_H__