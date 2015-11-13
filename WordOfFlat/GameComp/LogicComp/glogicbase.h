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
class LogicBase : public ComponentBase
{
protected:
	LogicSystem *m_pOwnerManager;
	GameEntity *m_pParent;

public:

	LogicBase(GameComponentType logicCompType) : ComponentBase(logicCompType)
						, m_pOwnerManager(nullptr)
						, m_pParent(nullptr) {}
	virtual ~LogicBase();

	inline void SetOwnerManager(LogicSystem *pOwner) {
		m_pOwnerManager = pOwner;
	}

	inline GameEntity* GetParent() {
		return m_pParent;
	}

	GameErrorCode ProcessLogic();
	
protected:
	GameErrorCode PhysicsProcess();

};


#endif //__GAME_LOGIC_COMPONENT_H__
