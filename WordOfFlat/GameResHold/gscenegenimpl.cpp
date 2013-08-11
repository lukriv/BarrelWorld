#include "gscenegenimpl.h"

GameErrorCode GameTestSceneGenerator::GenAnimals(const TGameWorldSegmentID& wrldID, wxVector<BasicEntityDef>& animals)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenCharacters(const TGameWorldSegmentID& wrldID, wxVector<BasicEntityDef>& characters)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenLandscape(const TGameWorldSegmentID& wrldID, wxVector<BasicEntityDef>& landscape)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	EntityDef entity;
	
	entity.m_entType = GAME_OBJECT_TYPE_STATIC_ENTITY;
	entity.m_geometryRef = 2;
	entity.m_physRef = 2;
	entity.m_textureRef = 2;
	entity.m_tranformation.p.Set(0.0f, -10.0f);
	entity.m_tranformation.q.Set(0.0f);
	entity.m_textureRepeat = true;
	landscape.push_back(entity);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenMoveableObj(const TGameWorldSegmentID& wrldID, wxVector<BasicEntityDef>& moveableObjs)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	EntityDef entity;
	
	entity.m_entType = GAME_OBJECT_TYPE_BASIC_MOVEABLE_ENTITY;
	entity.m_geometryRef = 1;
	entity.m_physRef = 1;
	entity.m_textureRef = 1;
	entity.m_tranformation.p.Set(0.0f, 2.0f);
	entity.m_tranformation.q.Set(0.0f);
	
	moveableObjs.push_back(entity);
	
	entity.m_entType = GAME_OBJECT_TYPE_BASIC_MOVEABLE_ENTITY;
	entity.m_geometryRef = 1;
	entity.m_physRef = 1;
	entity.m_textureRef = 1;
	entity.m_tranformation.p.Set(3.0f, 2.0f);
	entity.m_tranformation.q.Set(1.0f);
	
	moveableObjs.push_back(entity);
	
	return FWG_NO_ERROR;}

GameErrorCode GameTestSceneGenerator::GenStaticObj(const TGameWorldSegmentID& wrldID, wxVector<BasicEntityDef>& staticObjs)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

void GameTestSceneGenerator::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameTestSceneGenerator::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	
	return refCount;
}
