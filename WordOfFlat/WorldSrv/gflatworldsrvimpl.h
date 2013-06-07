#ifndef __GAME_FLAT_WORLD_SERVER_IMPL_H__
#define __GAME_FLAT_WORLD_SERVER_IMPL_H__

#include <scopedptr.h>
#include "../GameSystem/gerror.h"
#include "../GameSystem/glog.h"
#include "gflatworld.h"
/*! This could store world object for 2D
 *  It could add new object. 
 *  It could simulate this part of world
 *  It could send updated objects from world
 * 
 */

class GameFlatWorldSrv : public IGameFlatWorld{
private:
	
	GameFlatWorldID m_wrldId;
	GameLoggerPtr m_spLogger;
	b2Vec2 m_gravity;
	wxScopedPtr<b2World> m_apWorld;
	wxVector<IGameObject*> m_objectsVec;
	bool m_isInitialized;
public:
	GameFlatWorldSrv() :m_refCount(1), 
							m_wrldId(GAME_FLAT_WRLD_UNKNOWN),
							 m_isInitialized(false) {}
	
	GameErrorCode Initialize(GameLogger* logger = NULL);
	
	
	GameErrorCode GenerateTestStaticWorld();
	
	GameLogger* GetLogger() {return m_spLogger.In()}
	
public:

	GameFlatWorldID GetFWId();
	
	GameErrorCode StepWorld();
	
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
	virtual GameFlatWorldID GetFWId();
	
	virtual GameErrorCode SimulationStep();
	virtual GameErrorCode DrawStep();
	virtual GameErrorCode EventStep();
	virtual GameErrorCode AIStep();
	
	virtual GameErrorCode GetUpdateList(GameUpdateStruct** &updList, wxDword &listSize);
}

#endif //__GAME_FLAT_WORLD_SERVER_IMPL_H__