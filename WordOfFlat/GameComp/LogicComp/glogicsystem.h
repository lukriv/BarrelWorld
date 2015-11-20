#ifndef __GAME_LOGIC_COMPONENT_MANAGER_H__0c__
#define __GAME_LOGIC_COMPONENT_MANAGER_H__0c__

#include <GameSystem/glog.h>
#include <GameSystem/gset.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>

class GameCompManager;
class LogicBase;
class GameEntity;
struct LogicDef;

class LogicSystem
{
private:
	typedef GameBasSet<LogicBase*> TLogicCompList;
private:
	GameLoggerPtr m_spLogger;
	TLogicCompList m_logicCompList;
	
	wxCriticalSection m_lockMgr;
public:
	LogicSystem(GameLogger *pLogger);
	virtual ~LogicSystem();
	
	inline GameLogger* GetLogger() { return m_spLogger; }
	
	GameErrorCode Initialize();
	
	
	/**
	 * \brief Add logic component in manager
	 * \param pLogicComp Logic component
	 */
	GameErrorCode AddLogicComp(LogicBase *pLogicComp);
	
	void RemoveLogicComp(LogicBase *pLogicComp);
	
	/**
	 * \brief Process all logic components in this manager
	 */
	GameErrorCode ProcessLogicStep();

};

#endif // __GAME_LOGIC_COMPONENT_MANAGER_H__0c__
