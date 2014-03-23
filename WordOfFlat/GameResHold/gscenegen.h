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
	virtual GameErrorCode GenerateLandscape() = 0;
	
};


#endif //__GAME_SCENE_GENERATOR_H__