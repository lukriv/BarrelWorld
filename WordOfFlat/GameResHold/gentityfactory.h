#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <wx/atomic.h>
#include "../GameSystem/refobjectsmptr.h"

#include "../GameObjects/gentityobj.h"
#include "../GameObjects/ganimation.h"
#include "gresholder.h"




class GameEntityFactory : IRefObject {
	wxAtomicInt m_refCount;
	RefObjSmPtr<GameResourceHolder> m_spResHolder;
	GameLoggerPtr m_spLogger;
public:
	GameEntityFactory() : m_refCount(1){}
	
	GameErrorCode Initialize(GameResourceHolder *pResHolder, GameLogger *pLogger);
	
	GameErrorCode CreateBasicEntity(const BasicEntityDef &entityDef, b2World &world, GameEntity &entity);
	GameErrorCode CreateAnimation(const AnimationDef& entityDef, GameAnimation &animation);
	
	
public:
 	virtual void addRef();
	virtual wxInt32 release();
};



#endif //__GAME_ENTITY_FACTORY_H__