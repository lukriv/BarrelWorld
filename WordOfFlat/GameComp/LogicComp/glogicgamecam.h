#ifndef __GAME_CAMERA_LOGIC_H__
#define __GAME_CAMERA_LOGIC_H__

 // Base class: LogicComponentBase
#include "glogicbase.h"
#include <bullet/LinearMath/btScalar.h>
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputfreecam.h"
#include "gmoveable.h"

class LogicGameCamera : public Moveable
{
	btScalar m_angleX;
	btScalar m_angleY;
	
	btScalar m_diffSinceLastFrameX;
	btScalar m_diffSinceLastFrameY;
	
	btScalar m_diffMoveVertical;
	btScalar m_diffMoveHorizontal;
public:
	LogicGameCamera(GameLogicSystem *pLogicSystem);
	~LogicGameCamera();
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	
	virtual GameErrorCode Update(float timeDiff) override;
	
	virtual GameErrorCode Load(wxXmlNode *pNode) override;
	virtual GameErrorCode Store(wxXmlNode *pParentNode) override;

};

#endif // __GAME_CAMERA_LOGIC_H__