#ifndef __GAME_FREE_CAMERA_LOGIC_TEST_H__
#define __GAME_FREE_CAMERA_LOGIC_TEST_H__

 // Base class: LogicComponentBase
#include "glogicbase.h"
#include <bullet/LinearMath/btScalar.h>
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputfreecam.h"
#include "gmoveable.h"

class LogicFreeCamera : public Moveable
{
	btScalar m_angleX;
	btScalar m_angleY;
	
	btScalar m_diffSinceLastFrameX;
	btScalar m_diffSinceLastFrameY;
	
	btScalar m_diffMoveVertical;
	btScalar m_diffMoveHorizontal;
public:
	LogicFreeCamera(GameLogicSystem *pLogicSystem);
	~LogicFreeCamera();
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	
	virtual GameErrorCode Update(float timeDiff) override;
	
	virtual GameErrorCode Load(wxXmlNode *pNode) override;
	virtual GameErrorCode Store(wxXmlNode *pParentNode) override;

};

#endif // __GAME_FREE_CAMERA_LOGIC_TEST_H__