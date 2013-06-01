#ifndef __GAME_FLAT_WORLD_SERVER_IMPL_H__
#define __GAME_FLAT_WORLD_SERVER_IMPL_H__

#include <scopedptr.h>
#include "../GameSystem/gerror.h"
#include "../GameSystem/glog.h"
/*! This could store world object for 2D
 *  It could add new object. 
 *  It could simulate this part of world
 *  It could send updated objects from world
 * 
 */

class GameFlatWorldSrv : public IGameFlatWorldSrv{
private:
	
	wxAtomicInt m_refCount;
	GameFlatWorldID m_wrldId;
	GameLoggerPtr m_spLogger;
	b2Vec2 m_gravity;
	wxScopedPtr<b2World> m_apWorld;
	std::set<IGameObjectSrv*> m_objectMap;
	bool m_isInitialized;
public:
	GameFlatWorldSrv() :m_refCount(1), 
							m_wrldId(GAME_FLAT_WRLD_UNKNOWN),
							 m_isInitialized(false) {}
	
	GameErrorCode Initialize(GameLogger* logger = NULL);
	
	
	GameErrorCode GenerateTestStaticWorld();
	
public:

	GameFlatWorldID GetFWId();
	
	GameErrorCode StepWorld();
	
	GameErrorCode AddNewObject(IGameObjectSrv *object);
	GameErrorCode SetWorldSize(const b2Vec2 &LLpoint, const b2Vec2 &RUpoint);
	
	GameErrorCode LoadWorld(const wxChar* worldName);
	
public:
	virtual void addRef();
	virtual wxInt32 release();
}

#endif //__GAME_FLAT_WORLD_SERVER_IMPL_H__