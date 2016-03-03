#ifndef __GAME_FREE_CAMERA_LOGIC_TEST_H__
#define __GAME_FREE_CAMERA_LOGIC_TEST_H__

 // Base class: LogicComponentBase
#include <GameComp/logicComp/glogiccontroller.h>
#include <bullet/LinearMath/btScalar.h>
#include <GameComp/transformComp/gtranscomp.h>
#include "ginputfreecam.h"


class LogicFreeCamera : public LogicController
{
	RefObjSmPtr<TransformComponent> m_spTransform;
    RefObjSmPtr<FreeCameraInput> m_spInput;
	
	btScalar m_angleX;
	btScalar m_angleY;
	
	btScalar m_diffSinceLastFrameX;
	btScalar m_diffSinceLastFrameY;
	
	btScalar m_diffMoveVertical;
	btScalar m_diffMoveHorizontal;
	btScalar m_diffWheel;
public:
	LogicFreeCamera();
	~LogicFreeCamera();
	
	virtual GameErrorCode Update(float timeDiff) override;
	
	GameErrorCode Initialize(TransformComponent* pTransform, FreeCameraInput* pFreeCamInput);

};

#endif // __GAME_FREE_CAMERA_LOGIC_TEST_H__