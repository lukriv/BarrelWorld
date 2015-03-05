#include "gentityfactory.h"

#include <wx/scopedptr.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <GameSystem/refobjectsmptr.h>
#include <GameComp/RenderComp/grendercomp.h>
#include <GameComp/RenderComp/grenderobj.h>
#include <GameComp/InputComp/ginputcomp.h>
#include <GameComp/LogicComp/glogicman.h>
#include <GameSystem/new.h>








GameErrorCode GameEntityFactory::CreateAllEntities(GameDefinitionHolder &defHolder, GameCompManager& compMgr)
{
	TEntityDefMap::Iterator iter;
	
	for (iter = defHolder.m_entityDefs.Begin(); iter != defHolder.m_entityDefs.End(); iter++)
	{
		GameEntity *pEntity = nullptr;
		FWG_RETURN_FAIL(CreateEntity(*(iter->second), compMgr, pEntity));
	}
	
	return FWG_NO_ERROR;
}


GameErrorCode GameEntityFactory::CreateEntity( EntityDef& entityDef, GameCompManager& compMgr, GameEntity *&pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	FWG_RETURN_FAIL(compMgr.GetEntityManager().CreateEntity(entityDef.GetName(), pEntity));
	
	if(compMgr.GetRenderManager().GetOgreSceneManager() != nullptr)
	{
		
		if(!entityDef.m_transformation.IsEmpty())
		{
			if(FWG_FAILED(result = compMgr.GetTransformManager().CreateTransformComponent(*entityDef.m_transformation, pEntity)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
		}
		

		if(!entityDef.m_renderDef.IsEmpty())
		{
			if(FWG_FAILED(result = compMgr.GetRenderManager().CreateRenderComponent(*entityDef.m_renderDef, pEntity)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create Render component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
		}
		
		if(!entityDef.m_physDef.IsEmpty())
		{
			if(FWG_FAILED(result = compMgr.GetPhysicsManager().CreatePhysicsComponent(*entityDef.m_physDef, pEntity)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create physics component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
		}
		

		
		if(!entityDef.m_inputDef.IsEmpty())
		{
			RefObjSmPtr<InputComponent> spInputComp;
			if(FWG_FAILED(result = compMgr.GetInputSystem().CreateAndRegisterInputComponent(*entityDef.m_inputDef, spInputComp.OutRef())))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create input component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
			
			if(FWG_FAILED(result = pEntity->AddComponent(spInputComp)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Add input component to entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
		}
		
		if(!entityDef.m_logicDef.IsEmpty())
		{
			if(FWG_FAILED(result = compMgr.GetLogicManager().CreateLogicComp(*entityDef.m_logicDef, pEntity)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Create logic component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL );
				return result;
			}
		}
		
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameEntityFactory::CreateTerrain(GameDefinitionHolder& defHolder, GameCompManager& compMgr)
{
	if(!defHolder.m_spTerrain.IsEmpty())
	{
		return compMgr.GetTerrainManager().CreateTerrainGroup(*defHolder.m_spTerrain);
	}
	
	return FWG_NO_ERROR;
}


