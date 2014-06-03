#ifndef __GAME_LOGIC_MANUAL_TEST_H__
#define __GAME_LOGIC_MANUAL_TEST_H__

 // Base class: LogicComponentBase
#include "glogiccomp.h"

class LogicManualTest : public LogicComponentBase
{
public:
	LogicManualTest();
	~LogicManualTest();

protected:
	virtual GameErrorCode UserLogic();
	
protected:
	GameErrorCode ProcessInput();
};

#endif // __GAME_LOGIC_MANUAL_TEST_H__
