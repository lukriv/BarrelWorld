#ifndef __DYNAMIC_ENTITY_CREATOR_H__
#define __DYNAMIC_ENTITY_CREATOR_H__


class GameEntityCreator : public IRefObject {
	wxAtomicInt m_refCount;
	RefObjSmPtr<GameEntityFactory> m_spEntityFactory;
	b2World m_pWorld;
public:
	GameEntityCreator() : m_refCount(1), m_pWorld(NULL) {}
	
	GameErrorCode Initialize(GameEntityFactory* pEntityFactory, b2World* pWorld);
	
	GameEntityBase* CreateTestingOrb(const sf::Vector2f& displayCoords);
	
public:
	virtual void addRef();
	virtual wxInt32 release();
};


#endif //__DYNAMIC_ENTITY_CREATOR_H__