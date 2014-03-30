#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "gtranscomp.h"
#include "grendercomp.h"
#include "ganimatormgr.h"


class GameEntity {
private:
	TransformComponent* m_pTransformComp;
	RenderComponent* m_renderComp;
	AnimatorComponent* m_animatorComp;

public:
	GameEntity() {}
	
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
