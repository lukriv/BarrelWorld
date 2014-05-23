#include "gentity.h"

//////////////////////////
// inline implementation
//////////////////////////
void GameEntity::SetRenderComp(RenderComponent *pRenderComp) 
{
	if( m_pRenderComp != NULL)
	{
		m_pRenderComp->SetParent(NULL); // remove old parent
	}
	m_pRenderComp = pRenderComp;
	
	if(m_pRenderComp != NULL)
	{
		m_pRenderComp->SetParent(this); // set new parent
		
		// set connectivity with other components
		if(m_pTransformComp != NULL)
		{
			m_pTransformComp->Connect(*m_pRenderComp);
		}
	}
}

void GameEntity::SetAnimatorComp(AnimatorComponent* pAnimatorComp)
{
	m_pAnimatorComp = pAnimatorComp;
}

void GameEntity::SetTransformComp(TransformComponent* pTransComp)
{
	m_pTransformComp = pTransComp;
	
	// if transform is NULL return 
	if(m_pTransformComp == NULL) return;
	
	// connect this component with other components - if it make sense
	if(m_pRenderComp != NULL)
	{
		m_pTransformComp->Connect(*m_pRenderComp);
	}
	
}
