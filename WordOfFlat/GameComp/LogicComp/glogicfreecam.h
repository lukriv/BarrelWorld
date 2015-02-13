#ifndef __GAME_FREE_CAMERA_LOGIC_TEST_H__
#define __GAME_FREE_CAMERA_LOGIC_TEST_H__

 // Base class: LogicComponentBase
#include <bullet/LinearMath/btScalar.h>
#include "glogiccomp.h"
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputcomp.h"



class LogicFreeCamera : public LogicComponentBase
{
	RefObjSmPtr<TransformComponent> m_spTransform;
	RefObjSmPtr<InputComponent> m_spInput;
	btScalar m_angleX;
	btScalar m_angleY;
public:
	LogicFreeCamera();
	~LogicFreeCamera();
	
	GameErrorCode Initialize(GameEntity *pEntity);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity) override;
	virtual	GameErrorCode Update() override;
	
	

protected:
	virtual GameErrorCode UserLogic();
	
protected:
	GameErrorCode ProcessInput();
};

#endif // __GAME_FREE_CAMERA_LOGIC_TEST_H__