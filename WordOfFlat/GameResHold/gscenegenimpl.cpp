#include "gscenegenimpl.h"

GameErrorCode GameSceneGenerator::GenAnimals(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& animals)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameSceneGenerator::GenCharacters(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& characters)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameSceneGenerator::GenLandscape(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& landscape)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameSceneGenerator::GenMoveableObj(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& moveableObjs)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

GameErrorCode GameSceneGenerator::GenStaticObj(const TGameWorldSegmentID& wrldID, wxVector<EntityDef>& staticObjs)
{
	FWG_UNREFERENCED_PARAMETER(wrldID);
	return FWG_NO_ERROR;
}

void GameSceneGenerator::addRef()
{
	wxAtomicInc(m_refCount);
}

wxInt32 GameSceneGenerator::release()
{
	wxInt32 refCount = wxAtomicDec(m_refCount);
	if (refCount == 0)
	{
		delete this;
	}
	
	return refCount;
}
