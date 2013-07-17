#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <wx/atomic.h>
#include "../GameSystem/refobjectsmptr.h"

#include "../GameObjects/gentityobj.h"
#include "gresholder.h"
#include "gscenegen.h"

class GameEntityFactory : IRefObject {
	wxAtomicInt m_refCount;
	RefObjSmPtr<GameResourceHolder> m_spResHolder;
	GameLoggerPtr m_spLogger;
private:
	GameErrorCode CreateEntityBasic(const EntityDef &entityDef, b2World &world, GameEntityBase *&pEntity);
	GameErrorCode CreateEntityGroup(const EntityDef &entityDef, b2World &world, GameEntityBase *&pEntity);
public:
	GameEntityFactory() : m_refCount(1){}
	
	GameErrorCode Initialize(GameResourceHolder *pResHolder, GameLogger *pLogger);
	
	GameErrorCode CreateEntity(const EntityDef &entityDef, b2World &world, GameEntityBase *&pEntity);
	
public:
 	virtual void addRef();
	virtual wxInt32 release();
};



#endif //__GAME_ENTITY_FACTORY_H__