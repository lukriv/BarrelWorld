#ifndef __GAME_CAMERA_LOGIC_H__
#define __GAME_CAMERA_LOGIC_H__

 // Base class: LogicComponentBase
#include <bullet/LinearMath/btScalar.h>
#include <GameComp/LogicComp/glogiccontroller.h>
#include <GameComp/transformComp/gtranscomp.h>
#include "ginputfreecam.h"

class GameCompManager;


class LogicGameCamera : public LogicController
{
	GameCompManager *m_pCompMgr;
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
	LogicGameCamera();
	~LogicGameCamera();
	
	GameErrorCode Initialize(TransformComponent* pTransform, FreeCameraInput* pFreeCamInput, GameCompManager *pCompMgr);
	
	virtual GameErrorCode Update(float timeDiff) override;
	
	
	
private:
	void UpdateCameraAngle();
	GameErrorCode SendUpdateMessage();
	
	GameErrorCode CreateBox();

};

#endif // __GAME_CAMERA_LOGIC_H__