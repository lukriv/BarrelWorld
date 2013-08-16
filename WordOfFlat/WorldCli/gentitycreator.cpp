#include "gentitycreator.h"
#include "../GameObjects/gtransformable.h"

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

GameErrorCode GameEntityCreator::CreateTestingOrb(const sf::Vector2f& displayCoords, GameEntity& entity)
{
	AnimationDef animEntDef;
	
	
	animEntDef.m_frameRefs.push_back(3);
	animEntDef.m_frameDurations.push_back(sf::milliseconds(500));
	
	animEntDef.m_frameRefs.push_back(4);
	animEntDef.m_frameDurations.push_back(sf::milliseconds(500));
	
	
	//animEntDef.m_tranformation.p = GameTransform::Display2PhysicsPosition(displayCoords);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntityCreator::Initialize(GameEntityFactory* pEntityFactory, b2World* pWorld)
{
	m_spEntityFactory = pEntityFactory;
	m_pWorld = pWorld;
	return FWG_NO_ERROR;
}
