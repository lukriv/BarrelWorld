#ifndef __GAME_SCENE_GENERATOR_H__
#define __GAME_SCENE_GENERATOR_H__


#include <wx/vector.h>
#include <Box2D/Box2D.h>
#include "../GameSystem/refobject.h"
#include "../GameSystem/gerror.h"
#include "../GameObjects/gobjdef.h"


#include "gentityfactory.h"


typedef wxDword TGameWorldSegmentID;

class IGameSceneGenerator : public IRefObject {
public:
	virtual GameErrorCode GenLandscape(const TGameWorldSegmentID &wrldID, wxVector<RenderCompDef> &landscape) = 0;
	virtual GameErrorCode GenStaticObj(const TGameWorldSegmentID &wrldID, wxVector<RenderCompDef> &staticObjs) = 0;
	virtual GameErrorCode GenMoveableObj(const TGameWorldSegmentID &wrldID, wxVector<RenderCompDef> &moveableObjs) = 0;
	virtual GameErrorCode GenAnimals(const TGameWorldSegmentID &wrldID, wxVector<RenderCompDef> &animals) = 0;
	virtual GameErrorCode GenCharacters(const TGameWorldSegmentID &wrldID, wxVector<RenderCompDef> &characters) = 0;
	
};


#endif //__GAME_SCENE_GENERATOR_H__