#ifndef __GAME_SCENE_GENERATOR_H__
#define __GAME_SCENE_GENERATOR_H__


#include <wx/vector.h>
#include <Box2D/Box2D.h>
#include "../GameSystem/refobject.h"
#include "../GameSystem/gerror.h"
#include "../GameObjects/gameobjdef.h"





typedef wxDword TGameWorldSegmentID;

class IGameSceneGenerator : public IRefObject {
public:
	virtual GameErrorCode GenLandscape(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &landscape) = 0;
	virtual GameErrorCode GenStaticObj(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &staticObjs) = 0;
	virtual GameErrorCode GenMoveableObj(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &moveableObjs) = 0;
	virtual GameErrorCode GenAnimals(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &animals) = 0;
	virtual GameErrorCode GenCharacters(const TGameWorldSegmentID &wrldID, wxVector<EntityDef> &characters) = 0;
	
};


#endif //__GAME_SCENE_GENERATOR_H__