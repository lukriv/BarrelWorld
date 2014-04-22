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


GameErrorCode GameEntityFactory::CreateAllEntities(GameDefinitionHolder &defHolder, GameCompManager& compMgr)
{
	TEntityDefMap::Iterator iter;
	for (iter = defHolder.m_entityDefs.Begin(); iter != defHolder.m_entityDefs.End(); iter++)
	{
		
		GameEntity* pEntity = compMgr.GetEntityManager().CreateEntity(iter->second->m_entityName);
		if(pEntity == NULL)
		{
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
		
		FWG_RETURN_FAIL(CreateEntity(*(iter->second), compMgr, *pEntity));
	}
	
	return FWG_NO_ERROR;
}


GameErrorCode GameEntityFactory::CreateEntity( EntityDef& entityDef, GameCompManager& compMgr, GameEntity& entity)
{
	if(compMgr.GetRenderManager().GetOgreSceneManager() != NULL)
	{
		if(!entityDef.m_mesh.IsEmpty() && !entityDef.m_material.IsEmpty())
		{
			// render entity is defined
			Ogre::Entity *pEntity = NULL;
			RenderComponent *pRenderComp = NULL;
			
			if(!entityDef.m_entityName.IsEmpty())
			{
				pEntity = compMgr.GetRenderManager().GetOgreSceneManager()->createEntity(entityDef.m_entityName.ToStdString(), entityDef.m_mesh->m_name.ToStdString());
			} else {
				return FWG_E_NOT_IMPLEMENTED_ERROR;
			}
			
			//pEntity->setMaterialName(entityDef.m_material->m_name.ToStdString());
			pEntity->setMaterialName(entityDef.m_material->m_name.ToStdString());
			
			pRenderComp = compMgr.GetRenderManager().CreateEmptyRenderComponent();
			if(pRenderComp == NULL)
			{
				return FWG_E_MEMORY_ALLOCATION_ERROR;
			}
					
			pRenderComp->SetOgreEntity(pEntity);
			pRenderComp->SetParent(&entity);
			
			entity.SetRenderComp(pRenderComp);
						
		}
		
		if(!entityDef.m_transformation.IsEmpty())
		{
			TransformComponent *pTransform = NULL;
			Ogre::SceneNode *pSceneNode = compMgr.GetRenderManager().GetOgreSceneManager()->getRootSceneNode()->createChildSceneNode();
			pSceneNode->setPosition(entityDef.m_transformation->m_position);
			pSceneNode->setOrientation(entityDef.m_transformation->m_rotation);
			pSceneNode->setScale(entityDef.m_transformation->m_scale);
			
			FWG_RETURN_FAIL(GameNewChecked(pTransform));
			pTransform->SetSceneNode(pSceneNode);
			
			entity.SetTransformComp(pTransform);
		}
		
	}
	
	return FWG_NO_ERROR;
}
