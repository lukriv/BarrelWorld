#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__


#include <GameSystem/refobjectsmptr.h>

#include "TransformComp/gtranscomp.h"
#include "RenderComp/grendercomp.h"
#include "AnimatorComp/ganimatormgr.h"

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
	RefObjSmPtr<TransformComponent> m_pTransformComp;
	RefObjSmPtr<RenderComponent> m_pRenderComp;
	RefObjSmPtr<AnimatorComponent> m_pAnimatorComp;

public:
	GameEntity() {}
				
	void SetName(const wxString& name) { m_entityName.assign(name);}
	
	const wxString& GetName() const { return m_entityName; }
	
	void SetTransformComp(TransformComponent* pTransComp);
	void SetRenderComp(RenderComponent *pRenderComp);
	void SetAnimatorComp(AnimatorComponent *pAnimatorComp);

	
	inline TransformComponent* GetTransformComp() { return m_pTransformComp; }
	inline RenderComponent* GetRenderComp() { return m_pRenderComp; }
	inline AnimatorComponent* GetAnimatorComp() { return m_pAnimatorComp; }

};




#endif // __GAME_ENTITY_H__
