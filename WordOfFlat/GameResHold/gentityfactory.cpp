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
	wxScopedPtr<GameEntity> spEntity;
	wxDword i = 0;
	
	spEntity.reset(new (std::nothrow) GameEntity(entityDef.m_entType));
	if(spEntity.get() == NULL) {
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if (entityDef.m_textureRefs.size() == 1) {
		sf::Texture *pTexture = m_spResHolder->GetTexture(entityDef.m_textureRefs[0]);
		spEntity->SetTexture(pTexture);
	}
	
	
	
}

GameErrorCode GameEntityFactory::CreateEntityGroup(const EntityDef& entityDef, b2World& world, GameEntityBase*& pEntity)
{
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


