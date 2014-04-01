#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <wx/atomic.h>
#include "../GameSystem/refobjectsmptr.h"
#include "../GameSystem/glog.h"
#include "../GameComp/gcompmgr.h"
#include "gdefholder.h"



class GameEntityFactory : IRefObject {
public:
	struct InOutSystems {
		Ogre::SceneManager *m_pSceneMgr;
		
		InOutSystems() : m_pSceneMgr(NULL) {}
	};
	
private:
	wxAtomicInt m_refCount;
	GameLoggerPtr m_spLogger;
	
public:
	GameEntityFactory() : m_refCount(1){}
	
	GameErrorCode Initialize(GameLogger *pLogger);
	
	GameErrorCode CreateAllEntities(GameDefinitionHolder &defHolder, InOutSystems& inoutSys, GameCompManager& compMgr);
	
	GameErrorCode CreateEntity( EntityDef &entityDef, InOutSystems& inoutSys, GameEntity &entity);
	
public:
 	virtual void addRef();
	virtual wxInt32 release();
};



#endif //__GAME_ENTITY_FACTORY_H__