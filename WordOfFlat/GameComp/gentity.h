#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "TransformComp/gtranscomp.h"
#include "RenderComp/grendercomp.h"
#include "AnimatorComp/ganimatormgr.h"


class GameEntity {
private:
	TransformComponent* m_pTransformComp;
	RenderComponent* m_renderComp;
	AnimatorComponent* m_animatorComp;

public:
	GameEntity() : m_pTransformComp(NULL)
				, m_renderComp(NULL)
				, m_animatorComp(NULL) {}
	
	inline void SetTransformComp(TransformComponent* pTransComp) { m_pTransformComp = pTransComp; }
	inline void SetRenderComp(RenderComponent *pRenderComp);
	inline void SetAnimatorComp(AnimatorComponent *pAnimatorComp) { m_animatorComp = pAnimatorComp; }

	
	inline TransformComponent* GetTransformComp() { return m_pTransformComp; }
	inline RenderComponent* GetRenderComp() { return m_renderComp; }
	inline AnimatorComponent* GetAnimatorComp() { return m_animatorComp; }


};


//////////////////////////
// inline implementation
//////////////////////////
inline void GameEntity::SetRenderComp(RenderComponent *pRenderComp) 
{
	if( m_renderComp != NULL)
	{
		m_renderComp->SetParent(NULL); // remove old parent
	}
	m_renderComp = pRenderComp;
	if(m_renderComp != NULL)
	{
		m_renderComp->SetParent(this); // set new parent
	}
}

#endif // __GAME_ENTITY_H__
