#ifndef __DYNAMIC_ENTITY_CREATOR_H__
#define __DYNAMIC_ENTITY_CREATOR_H__

#include <wx/atomic.h>
#include <box2d/dynamics/b2World.h>
#include "../GameSystem/refobject.h"
#include "../GameSystem/refobjectsmptr.h"
#include "../GameResHold/gentityfactory.h"


class GameEntityCreator : public IRefObject {
	wxAtomicInt m_refCount;
	RefObjSmPtr<GameEntityFactory> m_spEntityFactory;
	b2World *m_pWorld;
public:
	GameEntityCreator() : m_refCount(1), m_pWorld(NULL) {}
	
	GameErrorCode Initialize(GameEntityFactory* pEntityFactory, b2World* pWorld);
	
	GameErrorCode CreateTestingOrb(const sf::Vector2f& displayCoords, GameEntity &entity);
	
public:
	virtual void addRef();
	virtual wxInt32 release();
};


#endif //__DYNAMIC_ENTITY_CREATOR_H__