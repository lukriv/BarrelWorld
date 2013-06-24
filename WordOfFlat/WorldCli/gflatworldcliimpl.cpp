#include "gflatworldcliimpl.h"


static const wxDword RESERVE_CONSTANT = 10000;

GameFlatWorldClient::GameFlatWorldClient() {
	m_entitiesList.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_entitiesList.begin(); iter != m_entitiesList.end(); iter++) { *iter = nullptr; }
	
	m_textureMap.reserve(RESERVE_CONSTANT);
	for(TGameTextureList::iterator iter = m_textureMap.begin(); iter != m_textureMap.end(); iter++) { *iter = nullptr; }
	
	m_geometryMap.reserve(RESERVE_CONSTANT); 
	for(TGameGeometryList::iterator iter = m_geometryMap.begin(); iter != m_geometryMap.end(); iter++) { *iter = nullptr; }
}


GameErrorCode GameFlatWorldClient::AIStep()
{
}

GameErrorCode GameFlatWorldClient::AddEntity(GameObjectId objId, GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	if (objId >= RESERVE_CONSTANT) return FWG_E_MEMORY_ALLOCATION_ERROR;
	if (m_geometryMap[objId] != nullptr) return FWG_E_INVALID_INDEX_ERROR;
	pEntity->SetObjID(objId);
	switch (pEntity->GetType())
	{
		case GAME_OBJECT_TYPE_DYNAMIC_ENTITY:
		case GAME_OBJECT_TYPE_KINEMATIC_ENTITY;
		case GAME_OBJECT_TYPE_STATIC_ENTITY:
		case GAME_OBJECT_TYPE_SENZOR_ENTITY:
		{
			GameEntity *pEnt = static_cast<GameEntity*> (pEntity);
			pEnt->CreateNewBody(m_apWorld.get());
		}
		break;
	}
	
	m_geometryMap[objId] = pEntity;
	return result;
}

GameErrorCode GameFlatWorldClient::AddGeometry(GameShapeId shapeId, IGameGeometry* pShape)
{
	if (shapeId >= RESERVE_CONSTANT) return FWG_E_MEMORY_ALLOCATION_ERROR;
	if (m_geometryMap[shapeId] != nullptr) return FWG_E_INVALID_INDEX_ERROR;
	m_geometryMap[shapeId] = pShape;
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::AddTexture(GameTextureId texId, sf::Texture* pTexture)
{
	if (texId >= RESERVE_CONSTANT) return FWG_E_MEMORY_ALLOCATION_ERROR;
	if (m_textureMap[texId] != nullptr) return FWG_E_INVALID_INDEX_ERROR;
	m_textureMap[texId] = pTexture;
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::DrawStep()
{
}

GameErrorCode GameFlatWorldClient::EventStep()
{
}

GameEntityBase* GameFlatWorldClient::GetEntity(GameObjectId objId)
{
}

GameFlatWorldID GameFlatWorldClient::GetFWId()
{
}

IGameGeometry* GameFlatWorldClient::GetGeometry(GameShapeId shapeId)
{
}

sf::Texture* GameFlatWorldClient::GetTexture(GameTextureId texId)
{
}

GameErrorCode GameFlatWorldClient::GetUpdateList(GameUpdateStruct**& updList, wxDword& listSize)
{
}

GameErrorCode GameFlatWorldClient::SetWorldSize(const b2AABB wrldAABB)
{
}

GameErrorCode GameFlatWorldClient::SimulationStep()
{
}

GameFlatWorldClient::~GameFlatWorldClient()
{
}

GameErrorCode GameFlatWorldClient::Initialize(GameLogger* pLogger)
{
	pLogger.addRef();
	m_spLogger.Attach(pLogger);
	m_isInitialized = true;
	return FWG_NO_ERROR;
}
