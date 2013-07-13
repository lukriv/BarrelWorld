#include "gflatworldcliimpl.h"
#include "../GameSystem/gdefs.h"

static const wxDword RESERVE_CONSTANT = 10000;

GameFlatWorldClient::GameFlatWorldClient() : m_wrldId(0), m_pRenderTarget(NULL) {
	m_characters.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_characters.begin(); iter != m_characters.end(); iter++) { *iter = NULL; }
	
	m_landscape.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_landscape.begin(); iter != m_landscape.end(); iter++) { *iter = NULL; }
	
	m_moveAbleObj.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_moveAbleObj.begin(); iter != m_moveAbleObj.end(); iter++) { *iter = NULL; }
	
	m_senzors.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_senzors.begin(); iter != m_senzors.end(); iter++) { *iter = NULL; }
	
	m_staticObj.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_staticObj.begin(); iter != m_staticObj.end(); iter++) { *iter = NULL; }
}



GameErrorCode GameFlatWorldClient::AIStep()
{
	return FWG_NO_ERROR;
}


GameErrorCode GameFlatWorldClient::DrawStep()
{
	TSceneGraph::iterator iter;
	GameSceneObject *pSceneObj = NULL;
	for (iter = m_objectMap.begin(); iter != m_objectMap.end(); iter++)
	{
		pSceneObj = *iter;
		pSceneObj->draw(*m_pRenderTarget, sf::RenderStates());
	}
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::EventStep()
{
	return FWG_NO_ERROR;
}

GameFlatWorldID GameFlatWorldClient::GetFWId()
{
	return m_wrldId;
}

GameErrorCode GameFlatWorldClient::GetUpdateList(GameUpdateStruct**& updList, wxDword& listSize)
{
	FWG_UNREFERENCED_PARAMETER(updList);
	FWG_UNREFERENCED_PARAMETER(listSize);
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::SetWorldSize(const b2AABB wrldAABB)
{
	FWG_UNREFERENCED_PARAMETER(wrldAABB);
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::SimulationStep()
{
	GameErrorCode result = FWG_NO_ERROR;
	m_apWorld->Step(m_timeStep, m_velocityIter, m_positionIter);
	
	TGameEntityList::iterator iter;
	
	for (iter = m_moveAbleObj.begin(); iter != m_moveAbleObj.end(); iter++)
	{
		(**iter).UpdatePosition();
	}
	
	return result;
}

GameFlatWorldClient::~GameFlatWorldClient()
{
}

GameErrorCode GameFlatWorldClient::Initialize(sf::RenderTarget *pTarget, GameLogger* pLogger )
{

	m_spLogger = pLogger;
	
	m_timeStep = 1.0f / 60.0f;
	m_velocityIter = 8;
	m_positionIter = 3;
	m_gravity.Set(0.0f, -10.0f);
	m_apWorld.reset(new (std::nothrow) b2World(m_gravity));
	
	m_pRenderTarget = pTarget;
	
	m_isInitialized = true;
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::AddCharacterEntity(GameObjectId objId, GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	if (objId >= RESERVE_CONSTANT) return FWG_E_MEMORY_ALLOCATION_ERROR;
	pEntity->SetObjID(objId);
	
	m_characters[objId] = pEntity;
	
	m_objectMap.push_back(pEntity);

	return result;
}

GameErrorCode GameFlatWorldClient::AddLandscapeEntity(GameObjectId objId, GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	if (objId >= RESERVE_CONSTANT) return FWG_E_MEMORY_ALLOCATION_ERROR;
	pEntity->SetObjID(objId);
	
	m_landscape[objId] = pEntity;
	
	m_objectMap.push_back(pEntity);

	return result;
}

GameErrorCode GameFlatWorldClient::AddMoveableEntity(GameObjectId objId, GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	if (objId >= RESERVE_CONSTANT) return FWG_E_MEMORY_ALLOCATION_ERROR;
	pEntity->SetObjID(objId);
	
	m_moveAbleObj[objId] = pEntity;
	
	m_objectMap.push_back(pEntity);

	return result;
}

GameErrorCode GameFlatWorldClient::AddSenzorEntity(GameObjectId objId, GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	if (objId >= RESERVE_CONSTANT) return FWG_E_MEMORY_ALLOCATION_ERROR;
	pEntity->SetObjID(objId);
	
	m_senzors[objId] = pEntity;
	
	m_objectMap.push_back(pEntity);

	return result;
}

GameErrorCode GameFlatWorldClient::AddStaticEntity(GameObjectId objId, GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	if (objId >= RESERVE_CONSTANT) return FWG_E_MEMORY_ALLOCATION_ERROR;
	pEntity->SetObjID(objId);
	
	m_staticObj[objId] = pEntity;
	
	m_objectMap.push_back(pEntity);

	return result;
}

GameEntityBase* GameFlatWorldClient::GetCharacterEntity(GameObjectId objId)
{
	return m_characters[objId];
}

GameEntityBase* GameFlatWorldClient::GetLandscapeEntity(GameObjectId objId)
{
	return m_landscape[objId];
}

GameEntityBase* GameFlatWorldClient::GetMoveableEntity(GameObjectId objId)
{
	return m_moveAbleObj[objId];
}

GameEntityBase* GameFlatWorldClient::GetSenzorEntity(GameObjectId objId)
{
	return m_senzors[objId];
}

GameEntityBase* GameFlatWorldClient::GetStaticEntity(GameObjectId objId)
{
	return m_staticObj[objId];
}
