#include "gentityfactory.h"

#include <wx/scopedptr.h>
#include "../GameObjects/gsfmlgeom.h"
#include "../GameObjects/grendercomp.h"

GameErrorCode GameEntityFactory::CreateBasicEntity(const RenderCompDef& entityDef, b2World& world, GameRenderComp& entity)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxVector<GameObjectID> physObjsList;
	wxVector<GameObjectID>::iterator iter;
	GameGeometryContainer *pGeomCont = NULL;
	wxDword i = 0;
	b2Body *pBody = NULL;
	
	if (entityDef.m_textureRef != GAME_OBJECT_ID_INVALID) {
		sf::Texture *pTexImage = m_spResHolder->GetTexture(entityDef.m_textureRef);
		if (pTexImage == NULL)
		{
			FWGLOG_WARNING_FORMAT(wxT("GameEntityFactory::CreateEntityBasic() : Texture image (%u) not found"),
				m_spLogger, entityDef.m_textureRef, FWGLOG_ENDVAL);
		} else {
			entity.SetBaseTexture(pTexImage);
			entity.GetTexture()->setRepeated(entityDef.m_textureRepeat);
		}
	}
	
	if (entityDef.m_geometryRef != GAME_OBJECT_ID_INVALID) {
		pGeomCont = m_spResHolder->GetGeometry(entityDef.m_geometryRef);
		if(pGeomCont == NULL) {
			FWGLOG_ERROR(wxT("GameEntityFactory::CreateEntityBasic() : Geometry not found"), m_spLogger);
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
	
	if (entityDef.m_physRef != GAME_OBJECT_ID_INVALID) {
		b2BodyDef *pBodyDef = NULL;
		if (FWG_FAILED(result = m_spResHolder->GetBodyDef(entityDef.m_physRef, pBodyDef, physObjsList)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameEntityFactory::CreateEntityBasic() : Get body def failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
		}
		pBodyDef->position.Set(entityDef.m_tranformation.p.x, entityDef.m_tranformation.p.y);
		pBodyDef->angle = entityDef.m_tranformation.q.GetAngle();
		pBodyDef->userData = static_cast<void*>(&entity);
		pBody = world.CreateBody(pBodyDef);
		entity.SetBody(pBody);
		FWGLOG_DEBUG_FORMAT(wxT("GameEntityFactory::CreateEntityBasic() : Body created position[%0.3f, %0.3f], angle[%0.3f]"),
			m_spLogger, entityDef.m_tranformation.p.x, entityDef.m_tranformation.p.y,
			entityDef.m_tranformation.q.GetAngle(),
			FWGLOG_ENDVAL);
	}
	
	for (iter = physObjsList.begin(); iter != physObjsList.end(); iter++)
	{
		b2FixtureDef *pFixtureDef = NULL;
		GameObjectID shapeId;
		IGameGeometry *pGeometry = NULL;
		wxScopedPtr<b2Shape> apShape;
		if (FWG_FAILED(result = m_spResHolder->GetFixtureDef(*iter, pFixtureDef, shapeId)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameEntityFactory::CreateEntityBasic() : Get body def failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		pGeomCont = m_spResHolder->GetGeometry(shapeId);
		if(pGeomCont != NULL)
		{
			apShape.reset(pGeomCont->CreatePhysShape());
		}
		
		pFixtureDef->shape = apShape.get();
		pBody->CreateFixture(pFixtureDef);
	}
	entity.SetPosition(entityDef.m_tranformation.p.x, entityDef.m_tranformation.p.y);
	entity.SetRotation(entityDef.m_tranformation.q.GetAngle());
	

	
	return FWG_NO_ERROR;
	
}

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

GameErrorCode GameEntityFactory::Initialize(GameResourceHolder* pResHolder, GameLogger* pLogger)
{
	m_spResHolder = pResHolder;
	m_spLogger = pLogger;
	return FWG_NO_ERROR;
}

GameErrorCode GameEntityFactory::CreateAnimation(const AnimationDef& animDef, GameAnimation& animation)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxDword i = 0;
	animation.Clear();
	for(i = 0; (i < animDef.m_frameRefs.size()) && (i < animDef.m_frameDurations.size()); ++i)
	{
		sf::Texture *pTexImage = m_spResHolder->GetTexture(animDef.m_frameRefs[i]);
		if (pTexImage == NULL)
		{
			FWGLOG_WARNING_FORMAT(wxT("GameEntityFactory::CreateEntityBasic() : Texture image (%u) not found"),
				m_spLogger, animDef.m_frameRefs[i], FWGLOG_ENDVAL);
		} else {
			animation.AddFrame(*pTexImage, animDef.m_frameDurations[i]);
		}
	}
	animation.SetAnimType(animDef.m_animType);
	animation.SetRepeat(animDef.m_repeat);
	animation.SetEndless(animDef.m_endless);
	
	return FWG_NO_ERROR;
}



