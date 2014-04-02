#include "gentityfactory.h"

#include <wx/scopedptr.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include "../GameSystem/refobjectsmptr.h"
#include "../GameComp/RenderComp/grendercomp.h"
#include "../GameSystem/new.h"


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
	m_spLogger = pLogger;
	return FWG_NO_ERROR;
}


GameErrorCode GameEntityFactory::CreateAllEntities(GameDefinitionHolder& defHolder, InOutSystems& inoutSys, GameCompManager& compMgr)
{
	TEntityDefMap::Iterator iter;
	for (iter = defHolder.m_entityDefs.Begin(); iter != defHolder.m_entityDefs.End(); iter++)
	{
		
		GameEntity* pEntity = compMgr.GetEntityManager().CreateEntity(iter->second->m_entityName);
		if(pEntity == NULL)
		{
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
		
		FWG_RETURN_FAIL(CreateEntity(*(iter->second), inoutSys, *pEntity));
	}
	
	return FWG_NO_ERROR;
}


GameErrorCode GameEntityFactory::CreateEntity( EntityDef& entityDef, InOutSystems& inoutSys, GameEntity& entity)
{
	if(inoutSys.m_pSceneMgr != NULL)
	{
		if(!entityDef.m_mesh.IsEmpty() && !entityDef.m_material.IsEmpty())
		{
			// render entity is defined
			Ogre::Entity *pEntity = NULL;
			RenderComponent *pRenderComp = NULL;
			
			if(!entityDef.m_entityName.IsEmpty())
			{
				pEntity = inoutSys.m_pSceneMgr->createEntity(entityDef.m_entityName.ToStdString(), entityDef.m_mesh->m_name.ToStdString());
			} else {
				return FWG_E_NOT_IMPLEMENTED_ERROR;
			}
			
			//pEntity->setMaterialName(entityDef.m_material->m_name.ToStdString());
			pEntity->setMaterialName(entityDef.m_material->m_name.ToStdString());
			
			FWG_RETURN_FAIL(GameNewChecked(pRenderComp));
					
			pRenderComp->SetEntity(pEntity);
			pRenderComp->SetParent(&entity);
			
			entity.SetRenderComp(pRenderComp);
						
		}
		
		if(!entityDef.m_transformation.IsEmpty())
		{
			TransformComponent *pTransform = NULL;
			Ogre::SceneNode *pSceneNode = inoutSys.m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
			pSceneNode->setPosition(entityDef.m_transformation->m_position);
			pSceneNode->setOrientation(entityDef.m_transformation->m_rotation);
			pSceneNode->setScale(entityDef.m_transformation->m_scale);
			
			FWG_RETURN_FAIL(GameNewChecked(pTransform));
			pTransform->SetSceneNode(pSceneNode);
			
			entity.SetTransformComp(pTransform);
			if(entity.GetRenderComp() != NULL)
			{
				pSceneNode->attachObject(entity.GetRenderComp()->GetEntity());
			}
		}
		
	}
	
	return FWG_NO_ERROR;
}
