#include "gflatworldcliimpl.h"


static const wxDword RESERVE_CONSTANT = 100;

GameFlatWorldClient::GameFlatWorldClient() {
	m_entitiesList.reserve(RESERVE_CONSTANT);
	m_textureMap.reserve(RESERVE_CONSTANT);
	m_geometryMap.reserve(RESERVE_CONSTANT); 
}


GameErrorCode GameFlatWorldClient::AIStep()
{
}

GameErrorCode GameFlatWorldClient::AddEntity(GameObjectId objId, GameEntityBase* pEntity)
{
}

GameErrorCode GameFlatWorldClient::AddGeometry(GameShapeId shapeId, IGameGeometry* pShape)
{
}

GameErrorCode GameFlatWorldClient::AddTexture(GameTextureId texId, sf::Texture* pTexture)
{
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
