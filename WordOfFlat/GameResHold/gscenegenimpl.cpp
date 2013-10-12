#include "gscenegenimpl.h"

GameErrorCode GameTestSceneGenerator::GenAnimals(const TGameWorldSegmentID& wrldID, wxVector<RenderCompDef>& animals)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenCharacters(const TGameWorldSegmentID& wrldID, wxVector<RenderCompDef>& characters)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenLandscape(const TGameWorldSegmentID& wrldID, wxVector<RenderCompDef>& landscape)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	RenderCompDef entity;
	
	entity.m_geometryRef = 2;
	entity.m_physRef = 2;
	entity.m_textureRef = 2;
	entity.m_tranformation.p.Set(0.0f, -10.0f);
	entity.m_tranformation.q.Set(0.0f);
	entity.m_textureRepeat = true;
	landscape.push_back(entity);
	
	return FWG_NO_ERROR;
}

GameErrorCode GameTestSceneGenerator::GenMoveableObj(const TGameWorldSegmentID& wrldID, wxVector<RenderCompDef>& moveableObjs)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	RenderCompDef entity;
	
	entity.m_geometryRef = 1;
	entity.m_physRef = 1;
	entity.m_textureRef = 1;
	entity.m_tranformation.p.Set(0.0f, 2.0f);
	entity.m_tranformation.q.Set(0.0f);
	
	moveableObjs.push_back(entity);
	
	entity.m_geometryRef = 1;
	entity.m_physRef = 1;
	entity.m_textureRef = 1;
	entity.m_tranformation.p.Set(3.0f, 2.0f);
	entity.m_tranformation.q.Set(1.0f);
	
	moveableObjs.push_back(entity);
	
	return FWG_NO_ERROR;}

GameErrorCode GameTestSceneGenerator::GenStaticObj(const TGameWorldSegmentID& wrldID, wxVector<RenderCompDef>& staticObjs)
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
