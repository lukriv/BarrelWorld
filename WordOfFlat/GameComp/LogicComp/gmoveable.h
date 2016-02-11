#ifndef __GAME_SIMPLE_MOVE_LOGIC_H__
#define __GAME_SIMPLE_MOVE_LOGIC_H__

// Base class: LogicComponentBase
#include "glogicbase.h"
#include "../transformComp/gtranscomp.h"
#include "../inputComp/ginputcomp.h"

class Moveable : public LogicBase, public ComponentBase
{
protected:
    RefObjSmPtr<TransformComponent> m_spTransform;
    RefObjSmPtr<InputComponent> m_spInput;
public:
    
    Moveable(GameLogicSystem *pLogicSystem);
    ~Moveable();
	
	GameErrorCode Initialize(TransformComponent* pTransform, InputComponent* pFreeCamInput);
	
	virtual void Enable(bool enable) override;

};

#endif // __GAME_SIMPLE_MOVE_LOGIC_H__

