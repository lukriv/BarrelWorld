#ifndef __GAME_LOGIC_COMPONENT_MANAGER_H__0c__
#define __GAME_LOGIC_COMPONENT_MANAGER_H__0c__

#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>

class LogicComponentBase;

class LogicCompManager
{
private:
	typedef std::set<LogicComponentBase*> TLogicCompList;
private:
	TLogicCompList m_logicCompList;
public:
	LogicCompManager();
	virtual ~LogicCompManager();
	
	GameErrorCode CreateLogicComp(LogicComponentBase *&pLogicComp);
	
	void RemoveLogicComp(LogicComponentBase *pLogicComp);
	
	
	GameErrorCode ProcessLogicStep();

};

#endif // __GAME_LOGIC_COMPONENT_MANAGER_H__0c__
