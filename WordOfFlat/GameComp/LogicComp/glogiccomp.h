#ifndef __GAME_LOGIC_COMPONENT_H__
#define __GAME_LOGIC_COMPONENT_H__

#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobjectsmptr.h>

class GameEntity;
class LogicCompManager;

/*!
 * \class GameEntity
 * \author Lukas
 * \date 12.6.2013
 * \file gentityobj.h
 * \brief Geometric entity with state and transform
 */
class LogicComponentBase : public RefObjectImpl<IRefObject> {
protected:
	LogicCompManager *m_pOwnerManager;
	GameEntity *m_pParent;
	
public:
	// Render component can be created and destroyed only by render component manager
	LogicComponentBase() : m_pOwnerManager(nullptr)
						, m_pParent(nullptr) {}
	virtual ~LogicComponentBase();

	inline void SetOwnerManager(LogicCompManager *pOwner) { m_pOwnerManager = pOwner; }
	
	inline void SetParent(GameEntity *pParent) { m_pParent = pParent; }
	inline GameEntity* GetParent() { return m_pParent; }
	
	GameErrorCode ProcessLogic();
	
	
protected:
	virtual GameErrorCode UserLogic() = 0;

protected:
	GameErrorCode PhysicsProcess();
	
};


#endif //__GAME_LOGIC_COMPONENT_H__