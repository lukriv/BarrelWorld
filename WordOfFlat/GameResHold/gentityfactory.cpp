#include "gentityfactory.h"

#include <wx/scopedptr.h>
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreSceneNode.h"
#include "../GameObjects/grendercomp.h"


void GameEntityFactory::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameEntityFactory::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	return refCount;
}

GameErrorCode GameEntityFactory::Initialize(GameLogger* pLogger)
{
	m_spResHolder = pResHolder;
	m_spLogger = pLogger;
	return FWG_NO_ERROR;
}


GameErrorCode GameEntityFactory::CreateAllEntities(GameDefinitionHolder& defHolder, GameCompManager& compMgr)
{
	TEntityDefMap::Iterator iter;
	for (iter = defHolder.m_entityDefs.Begin(); iter != defHolder.m_entityDefs.End(); iter++)
	{
		
		GameEntity* pEntity = compMgr.GetEntityManager().CreateEntity(iter->second->m_entityName);
		if(pEntity == NULL)
		{
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
		
		FWG_RETURN_FAIL(CreateEntity(*(iter->second), *pEntity));
	}
	
	return FWG_NO_ERROR;
}


GameErrorCode GameEntityFactory::CreateEntity( EntityDef& entityDef, GameEntity& entity)
{
	Ogre::Entity *pEntity = m_pSceneManager->createEntity("cc", "TestingCube");
	pEntity->setMaterialName("Test/ColourTest");
	Ogre::SceneNode *pSceneNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode();
	pSceneNode->setPosition(0,0,0);
	pSceneNode->attachObject(pEntity);
}
