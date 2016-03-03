#ifndef __GAME_LOGIC_MANUAL_TEST_H__
#define __GAME_LOGIC_MANUAL_TEST_H__

 // Base class: LogicComponentBase
#include <GameComp/transformComp/gtranscomp.h>
#include <GameComp/LogicComp/glogiccontroller.h>
#include <MainWindow/controllers/ginputchar.h>

class LogicManualTest : public LogicController
{
	RefObjSmPtr<TransformComponent> m_spTransform;
    RefObjSmPtr<CharacterInput> m_spInput;
	
	btScalar m_diffForwardBackward;
	btScalar m_diffLeftRight;
public:
	LogicManualTest();
	~LogicManualTest();
	
	GameErrorCode Initialize(TransformComponent* pTransform, CharacterInput* pFreeCamInput);
	
	virtual GameErrorCode Update(float timeDiff) override;
	
protected:
	GameErrorCode ProcessInput();
};

#endif // __GAME_LOGIC_MANUAL_TEST_H__
