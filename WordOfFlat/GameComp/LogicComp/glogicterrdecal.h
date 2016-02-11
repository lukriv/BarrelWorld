#ifndef __GAME_TERRAIN_DECAL_LOGIC_TEST_H__
#define __GAME_TERRAIN_DECAL_LOGIC_TEST_H__

 // Base class: LogicComponentBase
#include <bullet/LinearMath/btScalar.h>
#include <GameSystem/gerror.h>
#include "glogicbase.h"
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputcomp.h"

class GameTerrainManager;

class LogicTerrainDecal : public LogicBase
{
	RefObjSmPtr<TransformComponent> m_spTransform;
	RefObjSmPtr<InputComponent> m_spInput;
	GameTerrainManager *m_pTerrainMgr;
public:
	LogicTerrainDecal(GameLogicSystem *pLogicSystem);
	~LogicTerrainDecal();
	
	GameErrorCode Initialize(GameTerrainManager *pTerrainManager);
	
	GameErrorCode Load(wxXmlNode *pNode);
	GameErrorCode Store(wxXmlNode *pParentNode);

protected:
	virtual GameErrorCode UserLogic();
	
};

#endif // __GAME_FREE_CAMERA_LOGIC_TEST_H__