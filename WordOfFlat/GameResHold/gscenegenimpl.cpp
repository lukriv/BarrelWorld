#include "gscenegenimpl.h"

GameErrorCode GameTestSceneGenerator::GenAnimals(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& animals)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenCharacters(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& characters)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenLandscape(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& landscape)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	EntityDef entity;
	
	entity.m_entType = GAME_OBJECT_TYPE_STATIC_ENTITY;
	entity.m_geometryRefs.push_back(2);
	entity.m_physRefs.push_back(2);
	entity.m_textureRefs.push_back(2);
	entity.m_tranformation.p.Set(0.0f, -10.0f);
	
	landscape.push_back(entity);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenMoveableObj(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& moveableObjs)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenStaticObj(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& staticObjs)
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
