#ifndef __GAME_ENTITY_FACTORY_H__
#define __GAME_ENTITY_FACTORY_H__

#include <wx/atomic.h>
#include "../GameSystem/refobjectsmptr.h"

#include "gresholder.h"




class GameEntityFactory : IRefObject {
	wxAtomicInt m_refCount;
	RefObjSmPtr<GameResourceHolder> m_spResHolder;
	GameLoggerPtr m_spLogger;
public:
	GameEntityFactory() : m_refCount(1){}
	
	GameErrorCode Initialize(GameResourceHolder *pResHolder, GameLogger *pLogger);
	
	GameErrorCode CreateEntity(const RenderCompDef &entityDef, b2World &world, GameRenderComp &entity);
	
public:
 	virtual void addRef();
	virtual wxInt32 release();
};



#endif //__GAME_ENTITY_FACTORY_H__