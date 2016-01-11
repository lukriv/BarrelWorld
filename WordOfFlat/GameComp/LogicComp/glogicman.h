#ifndef __GAME_LOGIC_MANUAL_TEST_H__
#define __GAME_LOGIC_MANUAL_TEST_H__

 // Base class: LogicComponentBase
#include "glogicbase.h"
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputcomp.h"
#include "gmoveable.h"

class LogicManualTest : public Moveable
{

public:
	LogicManualTest();
	~LogicManualTest();
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	
	

	virtual GameErrorCode Update() override;
	
	virtual GameErrorCode Load(wxXmlNode *pNode) override;
	virtual GameErrorCode Store(wxXmlNode *pParentNode) override;
	
protected:
	GameErrorCode ProcessInput();
};

#endif // __GAME_LOGIC_MANUAL_TEST_H__
