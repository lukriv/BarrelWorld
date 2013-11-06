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

GameErrorCode GameEntityCreator::CreateTestingOrb(const sf::Vector2f& displayCoords, GameRenderComp& entity)
{
	GameAnimation *pAnimation = NULL;
	TAnimationCacheItem cacheItem;
	
	cacheItem = m_animCache.find(ANIM_KEY_TESTING_ORB);
	if(cacheItem == m_animCache.end()) 
	{
		GameAnimation *pAnimation2 = NULL;
		AnimationDef animdef;

		animdef.m_frameRefs.push_back(3);
		animdef.m_frameRefs.push_back(4);
		animdef.m_frameRefs.push_back(5);
		animdef.m_frameDurations.push_back(sf::milliseconds(500));
		animdef.m_frameDurations.push_back(sf::milliseconds(500));
		animdef.m_frameDurations.push_back(sf::milliseconds(500));
		animdef.m_repeat = true;
		animdef.m_animType = GameAnimation::ANIM_TYPE_LINEAR;
		
		entitydef.m_geometryRef = 1;
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->CreateNewEntity(pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create new entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->CreateNewAnimation(pAnimation)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create new animation failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_spEntityFactory->CreateBasicEntity(entitydef, *m_pActualFlatWorldClient->GetPhysWorld(), *pEntity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Set entity failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_spEntityFactory->CreateAnimation(animdef, *pAnimation)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Set animation failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		if(FWG_FAILED(result = m_pActualFlatWorldClient->CreateNewAnimation(pAnimation2)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Create new animation2 failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		pAnimation2->SetLogger(m_pLogger);
		pAnimation->SetLogger(m_pLogger);
		if(FWG_FAILED(result = pAnimation->GenerateStaticAnimation(*pAnimation2, sf::milliseconds(100))))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateTestingWorld() : Generate static animation failed: 0x%08x"),
				m_pLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		pEntity->SetAnimation(pAnimation2);
		pEntity->SetBlendMode(sf::BlendAlpha);
		m_pActualFlatWorldClient->AddMoveableEntity(pEntity);
		
	}
	
	
	//animEntDef.m_tranformation.p = GameTransform::Display2PhysicsPosition(displayCoords);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntityCreator::Initialize(GameEntityFactory* pEntityFactory, b2World* pWorld)
{
	m_spEntityFactory = pEntityFactory;
	m_pWorld = pWorld;
	return FWG_NO_ERROR;
}
