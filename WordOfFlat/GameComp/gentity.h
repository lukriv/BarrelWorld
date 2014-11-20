#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__


#include <GameSystem/refobjectsmptr.h>

#include "TransformComp/gtranscomp.h"
#include "RenderComp/grendercomp.h"
#include "AnimatorComp/ganimatormgr.h"
#include "LogicComp/glogiccomp.h"
#include "InputComp/ginputcomp.h"

class GameEntity {
private:
	wxString m_entityName;
	RefObjSmPtr<TransformComponent> m_spTransformComp;
	RefObjSmPtr<RenderComponent> m_spRenderComp;
	RefObjSmPtr<AnimatorComponent> m_spAnimatorComp;
	RefObjSmPtr<LogicComponentBase> m_spLogicComp;
	RefObjSmPtr<InputComponent> m_spInputComp;

public:

	GameEntity() {}
				
	void SetName(const wxString& name) { m_entityName.assign(name);}
	
	const wxString& GetName() const { return m_entityName; }
	
	void SetTransformComp(TransformComponent* pTransComp);
	void SetRenderComp(RenderComponent *pRenderComp);
	void SetAnimatorComp(AnimatorComponent *pAnimatorComp);
	void SetLogicComp(LogicComponentBase *pLogicComp);
	void SetInputComp(InputComponent *pInputComp);
	
	inline TransformComponent* GetTransformComp() { return m_spTransformComp; }
	inline RenderComponent* GetRenderComp() { return m_spRenderComp; }
	inline AnimatorComponent* GetAnimatorComp() { return m_spAnimatorComp; }
	inline LogicComponentBase* GetLogicComp() { return m_spLogicComp; }
	inline InputComponent* GetInputComp() { return m_spInputComp; }
	
	
	GameErrorCode ReceiveMessage(TaskMessage& msg);
	GameErrorCode Update(GameEntityComponent entity = ENTITY_COMP_ALL);
	
};




#endif // __GAME_ENTITY_H__
