#ifndef __GAME_LOGIC_COMPONENT_MANAGER_H__0c__
#define __GAME_LOGIC_COMPONENT_MANAGER_H__0c__

#include <GameSystem/gset.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>


class LogicComponentBase;

class LogicCompManager
{
private:
	typedef GameBasSet<LogicComponentBase*> TLogicCompList;
private:
	GameLoggerPtr m_spLogger;
	TLogicCompList m_logicCompList;
public:
	LogicCompManager(GameLogger *pLogger);
	virtual ~LogicCompManager();
	
	GameErrorCode AddLogicComp(LogicComponentBase *pLogicComp);
	
	void RemoveLogicComp(LogicComponentBase *pLogicComp);
	
	GameErrorCode ProcessLogicStep();

};

#endif // __GAME_LOGIC_COMPONENT_MANAGER_H__0c__
