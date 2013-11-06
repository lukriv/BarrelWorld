#include "grendercmgr.h"

GameErrorCode GameRenderComponentMgr::CreateGenericComponent(const RenderCompDef& renderCompDef, GameObjectId& compId)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxVector<GameObjectID>::iterator iter;
	GameGeometryContainer *pGeomCont = NULL;
	wxDword i = 0;
	b2Body *pBody = NULL;
	
	
	if(FWG_FAILED(result = m_genObjMemory.AllocNewItem(i)))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameRenderComponentMgr::CreateGenericComponent() : allocate new render item failed: 0x%08x"),
			m_pLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	GameRenderComp& pRenderComp = *(m_genObjMemory.GetItem(i));
	
	// load texture
	if (renderCompDef.m_textureRef != GAME_OBJECT_ID_INVALID) {
		sf::Texture *pTexImage = m_spResHolder->GetTexture(renderCompDef.m_textureRef);
		if (pTexImage == NULL)
		{
			FWGLOG_WARNING_FORMAT(wxT("GameRenderComponentMgr::CreateGenericComponent() : Texture image (%u) not found"),
				m_spLogger, renderCompDef.m_textureRef, FWGLOG_ENDVAL);
		} else {
			pRenderComp.SetBaseTexture(pTexImage);
			pRenderComp.GetTexture()->setRepeated(renderCompDef.m_textureRepeat);
		}
	}
	
	// load geometry
	if (renderCompDef.m_geometryRef != GAME_OBJECT_ID_INVALID) {
		pGeomCont = m_spResHolder->GetGeometry(renderCompDef.m_geometryRef);
		if(pGeomCont == NULL) {
			FWGLOG_ERROR(wxT("GameRenderComponentMgr::CreateGenericComponent() : Geometry not found"), m_spLogger);
			return result;
		}
		wxScopedPtr<IGameGeometry> apGeom(new (std::nothrow) GameSFMLGeometry());
		if (apGeom.get() == NULL) return FWG_E_MEMORY_ALLOCATION_ERROR;
		if (FWG_FAILED(result = apGeom->Create(*pGeomCont)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameEntityFactory::CreateEntityBasic() : Geometry creation failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		entity.SetGeometry(apGeom.get());
		apGeom.release();
	}
	
	entity.SetPosition(renderCompDef.m_tranformation.p.x, renderCompDef.m_tranformation.p.y);
	entity.SetRotation(renderCompDef.m_tranformation.q.GetAngle());
	
	return FWG_NO_ERROR;
}

GameErrorCode GameRenderComponentMgr::CreateLandscapeObject(const RenderCompDef& entityDef, GameObjectId& compId)
{
}

GameErrorCode GameRenderComponentMgr::DeleteGenericObject(GameObjectId compId)
{
}

GameErrorCode GameRenderComponentMgr::DeleteLandscapeObject(GameObjectId compId)
{
}

GameErrorCode GameRenderComponentMgr::RenderGeneric()
{
}

GameErrorCode GameRenderComponentMgr::RenderLandscape()
{
}

GameErrorCode GameRenderComponentMgr::CreateBackgroundObject()
{
}

GameErrorCode GameRenderComponentMgr::DeleteBackGroundObject()
{
}

GameErrorCode GameRenderComponentMgr::DeleteGenericObject(GameObjectID compId)
{
}

GameErrorCode GameRenderComponentMgr::DeleteLandscapeObject(GameObjectID compId)
{
}

GameErrorCode GameRenderComponentMgr::GetBackgroundObject()
{
}

GameErrorCode GameRenderComponentMgr::GetGenericObject(GameObjectID compID)
{
}

GameErrorCode GameRenderComponentMgr::GetLandscapeObject(GameObjectID compID)
{
}

GameErrorCode GameRenderComponentMgr::RenderBackground()
{
}
