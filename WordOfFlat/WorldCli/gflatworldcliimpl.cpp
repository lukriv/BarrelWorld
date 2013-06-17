#include "gflatworldcliimpl.h"



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
