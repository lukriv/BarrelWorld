#ifndef __GAME_SIMPLE_MOVE_LOGIC_H__
#define __GAME_SIMPLE_MOVE_LOGIC_H__

// Base class: LogicComponentBase
#include "glogicbase.h"
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputhandler.h"

class LogicController : public LogicBase, public ComponentBase
{
public:
    
    LogicController();
    ~LogicController();
	
	
	virtual void Enable(bool enable) override;
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	
	virtual GameErrorCode Load(wxXmlNode *pNode) override;
	virtual GameErrorCode Store(wxXmlNode *pParentNode) override;

};

#endif // __GAME_SIMPLE_MOVE_LOGIC_H__

