#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <wx/atomic.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameSystem/glog.h>
#include <GameComp/gcompmgr.h>
#include <MainWindow/ginputsystem.h>
#include "gdefholder.h"



class GameEntityFactory : IRefObject {
private:
	wxAtomicInt m_refCount;
	GameLoggerPtr m_spLogger;
	RefObjSmPtr<GameInputSystem> m_spInputSystem;
	
public:
	GameEntityFactory() : m_refCount(1){}
	
	GameErrorCode Initialize(GameLogger *pLogger, GameInputSystem *pInputSystem);
	
	void Uninitialize();
	
	GameErrorCode CreateAllEntities(GameDefinitionHolder &defHolder, GameCompManager& compMgr);
	
	GameErrorCode CreateEntity( EntityDef &entityDef, GameCompManager& compMgr, GameEntity &entity);
	
public:
 	virtual void addRef();
	virtual wxInt32 release();
};



#endif //__GAME_ENTITY_FACTORY_H__