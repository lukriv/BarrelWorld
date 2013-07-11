#include "gentityfactory.h"


GameErrorCode GameEntityFactory::CreateEntity(const EntityDef& entityDef, b2World& world, GameEntityBase*& pEntity)
{
	switch(entityDef.m_entType)
	{
	case GAME_OBJECT_TYPE_STATIC_ENTITY:
	case GAME_OBJECT_TYPE_DYNAMIC_ENTITY:
	case GAME_OBJECT_TYPE_KINEMATIC_ENTITY:
	case GAME_OBJECT_TYPE_SENZOR_ENTITY:
		return CreateEntityBasic(entityDef, world, pEntity);
	case GAME_OBJECT_TYPE_ENTITY_GROUP:
		return CreateEntityGroup(entityDef, world, pEntity);
	default:
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
}

GameErrorCode GameEntityFactory::CreateEntityBasic(const EntityDef& entityDef, b2World& world, GameEntityBase*& pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxScopedPtr<GameEntity> spEntity;
	wxVector<GamePhysObjId> physObjsList;
	wxVector<GamePhysObjId>::iterator iter;
	wxDword i = 0;
	b2Body *pBody = NULL;
	
	spEntity.reset(new (std::nothrow) GameEntity(entityDef.m_entType));
	if(spEntity.get() == NULL) {
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if (entityDef.m_textureRefs.size() > 1) {
		sf::Texture *pTexture = m_spResHolder->GetTexture(entityDef.m_textureRefs[0]);
		spEntity->SetTexture(pTexture);
	}
	
	if (entityDef.m_geometryRefs.size() > 1) {
		
		spEntity->SetGeometry(m_spResHolder->GetGeometry(entityDef.m_geometryRefs[0]));
	}
	
	if (entityDef.m_physRefs.size() > 1) {
		b2BodyDef *pBodyDef = NULL;
		if (FWG_FAILED(result = m_spResHolder->GetBodyDef(entityDef.m_physRefs[0], pBodyDef, physObjsList)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameEntityFactory::CreateEntityBasic() : Get body def failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
		}
		pBodyDef->position.Set(entityDef.m_tranformation.p.x, entityDef.m_tranformation.p.y);
		pBodyDef->angle = entityDef.m_tranformation.q.GetAngle();
		pBodyDef->userData = static_cast<void*>(spEntity.get());
		pBody = world.CreateBody(pBodyDef);
		spEntity->SetBody(pBody);
	}
	
	for (iter = physObjsList.begin(); iter != physObjsList.end(); iter++)
	{
		b2FixtureDef *pFixtureDef = NULL;
		GameShapeId shapeId;
		IGameGeometry *pGeometry = NULL;
		wxScopedPtr<b2Shape> apShape;
		if (FWG_FAILED(result = m_spResHolder->GetFixtureDef(*iter, pFixtureDef, shapeId)))
		{
			FWGLOG_ERROR_FORMAT(wxT("GameEntityFactory::CreateEntityBasic() : Get body def failed: 0x%08x"),
				m_spLogger, result, FWGLOG_ENDVAL);
		}
		
		pGeometry = m_spResHolder->GetGeometry(shapeId);
		if(pGeometry != NULL)
		{
			apShape.reset(pGeometry->CreatePhysShape());
		}
		
		pFixtureDef.shape = apShape.get();
		pBody->CreateFixture(pFixtureDef);
	}
	
	pEntity = spEntity.release();
	
	return FWG_NO_ERROR;
	
}

GameErrorCode GameEntityFactory::CreateEntityGroup(const EntityDef& entityDef, b2World& world, GameEntityBase*& pEntity)
{
	FWG_UNREFERENCED_PARAMETER(entityDef);
	FWG_UNREFERENCED_PARAMETER(world);
	FWG_UNREFERENCED_PARAMETER(pEntity);
	return FWG_E_NOT_IMPLEMENTED_ERROR;
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


