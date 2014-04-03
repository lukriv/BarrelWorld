#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "TransformComp/gtranscomp.h"
#include "RenderComp/grendercomp.h"
#include "AnimatorComp/ganimatormgr.h"


class GameEntity {
private:
	TransformComponent* m_pTransformComp;
	RenderComponent* m_pRenderComp;
	AnimatorComponent* m_pAnimatorComp;

public:
	GameEntity() : m_pTransformComp(NULL)
				, m_pRenderComp(NULL)
				, m_pAnimatorComp(NULL) {}
	
	void SetTransformComp(TransformComponent* pTransComp);
	void SetRenderComp(RenderComponent *pRenderComp);
	void SetAnimatorComp(AnimatorComponent *pAnimatorComp);

	
	inline TransformComponent* GetTransformComp() { return m_pTransformComp; }
	inline RenderComponent* GetRenderComp() { return m_pRenderComp; }
	inline AnimatorComponent* GetAnimatorComp() { return m_pAnimatorComp; }

};




#endif // __GAME_ENTITY_H__
