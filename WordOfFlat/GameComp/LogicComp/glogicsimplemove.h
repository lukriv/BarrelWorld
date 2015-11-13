#ifndef __GAME_SIMPLE_MOVE_LOGIC_H__
#define __GAME_SIMPLE_MOVE_LOGIC_H__

 // Base class: LogicComponentBase
#include "glogicbase.h"
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputcomp.h"

class LogicSimpleMove : public LogicBase
{
	RefObjSmPtr<TransformComponent> m_spTransform;
	RefObjSmPtr<FreeCameraInput> m_spInput;
	btScalar m_angleX;
	btScalar m_angleY;
public:
	LogicFreeCamera();
	~LogicFreeCamera();
	
	GameErrorCode CreateComponent( TransformComponent *pTransform, FreeCameraInput *pFreeCamInput );
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	
	GameErrorCode Update();
	
	GameErrorCode Load(wxXmlNode *pNode);
	GameErrorCode Store(wxXmlNode *pParentNode);

protected:
	GameErrorCode ProcessInput();
};

#endif // __GAME_SIMPLE_MOVE_LOGIC_H__