#include "gentitycreator.h"

void GameEntityCreator::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameEntityCreator::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	return refCount;
}

GameEntityBase* GameEntityCreator::CreateTestingOrb(const sf::Vector2f& displayCoords)
{
	EntityDef entDef;
	GameEntityBase *pEntity = NULL;
	
	entDef.m_entType = GAME_OBJECT_TYPE_EFECT_ENTITY;
	entDef.m_geometryRefs.push_back(1);
	entDef.m_textureRefs.push_back(3);
	entDef.m_textureRefs.push_back(4);
	entDef.m_tranformation.p = GameTransform::Display2PhysicsPosition(displayCoords);
	
	m_spEntityFactory->CreateEntity(entDef, m_pWorld, pEntity);
	
	return pEntity;	
}

GameErrorCode GameEntityCreator::Initialize(GameEntityFactory* pEntityFactory, b2World* pWorld)
{
	m_spEntityFactory = pEntityFactory;
	m_pWorld = pWorld;
	return FWG_NO_ERROR;
}
