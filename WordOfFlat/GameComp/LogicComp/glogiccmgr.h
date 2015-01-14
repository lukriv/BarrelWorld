#ifndef __GAME_LOGIC_COMPONENT_MANAGER_H__0c__
#define __GAME_LOGIC_COMPONENT_MANAGER_H__0c__

#include <GameSystem/glog.h>
#include <GameSystem/gset.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>


class LogicComponentBase;
class GameEntity;
struct LogicDef;

class LogicCompManager
{
private:
	typedef GameBasSet<LogicComponentBase*> TLogicCompList;
private:
	GameLoggerPtr m_spLogger;
	TLogicCompList m_logicCompList;
	
	wxCriticalSection m_lockMgr;
public:
	LogicCompManager(GameLogger *pLogger);
	virtual ~LogicCompManager();
	
	inline GameLogger* GetLogger() { return m_spLogger; }
	
	/**
	 * \brief Add logic component in manager
	 * \param pLogicComp Logic component
	 */
	GameErrorCode AddLogicComp(LogicComponentBase *pLogicComp);
	
	
	GameErrorCode CreateLogicComp(LogicDef &logicDef, GameEntity *pEntity);
	
	void RemoveLogicComp(LogicComponentBase *pLogicComp);
	
	/**
	 * \brief Process all logic components in this manager
	 */
	GameErrorCode ProcessLogicStep();

};

#endif // __GAME_LOGIC_COMPONENT_MANAGER_H__0c__
