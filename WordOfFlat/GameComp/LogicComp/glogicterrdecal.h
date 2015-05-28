#ifndef __GAME_TERRAIN_DECAL_LOGIC_TEST_H__
#define __GAME_TERRAIN_DECAL_LOGIC_TEST_H__

 // Base class: LogicComponentBase
#include <bullet/LinearMath/btScalar.h>
#include "glogiccomp.h"
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputcomp.h"



class LogicTerrainDecal : public LogicComponentBase
{
	RefObjSmPtr<TransformComponent> m_spTransform;
	RefObjSmPtr<InputComponent> m_spInput;
	GameTerrainManager *m_pTerrainMgr;
public:
	LogicTerrainDecal();
	~LogicTerrainDecal();
	
	GameErrorCode Initialize(GameTerrainManager *pTerrainManager, GameEntity *pEntity);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity) override;
	virtual	GameErrorCode Update() override;
	
	GameErrorCode Load(wxXmlNode *pNode);
	GameErrorCode Store(wxXmlNode *pParentNode);

protected:
	virtual GameErrorCode UserLogic();
	
};

#endif // __GAME_FREE_CAMERA_LOGIC_TEST_H__