#include "gamesrvobjimpl.h"



void GameObjectSrv::SetDrawableObj(IGameObject* pDrawableObj)
{
}

GameErrorCode GameObjectSrv::Initialize(b2Body* pBody)
{
}

GameErrorCode GameObjectSrv::AddPart(b2FixtureDef& fixture)
{
	m_pBody->CreateFixture(&fixture);
	return FWG_NO_ERROR;
}
