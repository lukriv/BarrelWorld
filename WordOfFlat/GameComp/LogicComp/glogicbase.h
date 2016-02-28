#ifndef __GAME_LOGIC_COMPONENT_H__
#define __GAME_LOGIC_COMPONENT_H__

#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>
#include "../gcompbase.h"

class GameEntity;
class GameLogicSystem;

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
	GameLogicSystem *m_pLogicSystem;

public:

	LogicBase();
	virtual ~LogicBase();

	virtual GameErrorCode Update(float timeDiff) = 0;
	
	inline void SetLogicSystem(GameLogicSystem *pLogicSystem)
	{
		m_pLogicSystem = pLogicSystem;
	}
	
};


#endif //__GAME_LOGIC_COMPONENT_H__
