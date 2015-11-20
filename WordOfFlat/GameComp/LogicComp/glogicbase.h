#ifndef __GAME_LOGIC_COMPONENT_H__
#define __GAME_LOGIC_COMPONENT_H__

#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>
#include "../gcompbase.h"

class GameEntity;
class LogicSystem;

/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class LogicBase
{
protected:
	LogicSystem *m_pLogicSystem;

public:

	LogicBase(): m_pLogicSystem(nullptr) {}
	virtual ~LogicBase();

	inline void SetOwnerManager(LogicSystem *pLogicSystem) {
		m_pLogicSystem = pLogicSystem;
	}

	virtual GameErrorCode ProcessLogic() = 0;
	
};


#endif //__GAME_LOGIC_COMPONENT_H__
