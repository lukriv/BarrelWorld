#ifndef __GAME_LOGIC_MANUAL_TEST_H__
#define __GAME_LOGIC_MANUAL_TEST_H__

 // Base class: LogicComponentBase
#include "glogiccomp.h"
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputcomp.h"

class LogicManualTest : public LogicComponentBase
{
	RefObjSmPtr<TransformComponent> m_spTransform;
	RefObjSmPtr<InputComponent> m_spInput;
public:
	LogicManualTest();
	~LogicManualTest();
	
	GameErrorCode Initialize(GameEntity *pEntity);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity) override;
	virtual	GameErrorCode Update() override;
	
	

protected:
	virtual GameErrorCode UserLogic();
	
protected:
	GameErrorCode ProcessInput();
};

#endif // __GAME_LOGIC_MANUAL_TEST_H__
