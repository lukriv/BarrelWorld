#ifndef __GAME_SCENE_GENERATOR_IMPLEMENTATION_H__
#define __GAME_SCENE_GENERATOR_IMPLEMENTATION_H__

#include <wx/atomic.h>
#include "gscenegen.h"

#include "gentityfactory.h"

class GameTestSceneGenerator : public IGameSceneGenerator {
	wxAtomicInt m_refCount;
public:
	GameTestSceneGenerator () : m_refCount(1) {}

public:
	GameErrorCode GenLandscape(const TGameWorldSegmentID &wrldID, wxVector<BasicEntityDef> &landscape);
	GameErrorCode GenStaticObj(const TGameWorldSegmentID &wrldID, wxVector<BasicEntityDef> &staticObjs);
	GameErrorCode GenMoveableObj(const TGameWorldSegmentID &wrldID, wxVector<BasicEntityDef> &moveableObjs);
	GameErrorCode GenAnimals(const TGameWorldSegmentID &wrldID, wxVector<BasicEntityDef> &animals);
	GameErrorCode GenCharacters(const TGameWorldSegmentID &wrldID, wxVector<BasicEntityDef> &characters);
public:
	virtual void addRef();
	virtual wxInt32 release();	
	
};




#endif //__GAME_SCENE_GENERATOR_IMPLEMENTATION_H__