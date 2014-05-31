#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__


#include <GameSystem/refobjectsmptr.h>

#include "TransformComp/gtranscomp.h"
#include "RenderComp/grendercomp.h"
#include "AnimatorComp/ganimatormgr.h"
#include "LogicComp/glogiccomp.h"

enum GameEntityType {
	ENTITY_TYPE_NONE 					= 0,
	ENTITY_TYPE_CHARACTER_TOPDOWN 		= 1,
	ENTITY_TYPE_CHARACTER_FIRST_PERSON 	= 2,
	ENTITY_TYPE_CHARACTER_THIRD_PERSON 	= 3,
	ENTITY_TYPE_CHARACTER_AI			= 4,
	ENTITY_TYPE_CAMERA_FREE				= 5
};

class GameEntity {
private:
	wxString m_entityName;
	GameEntityType m_entityType;
	RefObjSmPtr<TransformComponent> m_spTransformComp;
	RefObjSmPtr<RenderComponent> m_spRenderComp;
	RefObjSmPtr<AnimatorComponent> m_spAnimatorComp;
	RefObjSmPtr<LogicComponentBase> m_spLogicComp;

public:
	GameEntity() {}
				
	void SetName(const wxString& name) { m_entityName.assign(name);}
	
	const wxString& GetName() const { return m_entityName; }
	
	void SetTransformComp(TransformComponent* pTransComp);
	void SetRenderComp(RenderComponent *pRenderComp);
	void SetAnimatorComp(AnimatorComponent *pAnimatorComp);
	void SetLogicComp(LogicComponentBase *pLogicComp);

	
	inline TransformComponent* GetTransformComp() { return m_spTransformComp; }
	inline RenderComponent* GetRenderComp() { return m_spRenderComp; }
	inline AnimatorComponent* GetAnimatorComp() { return m_spAnimatorComp; }
	inline LogicComponentBase* GetLogicComp() { return m_spLogicComp; }

};




#endif // __GAME_ENTITY_H__
