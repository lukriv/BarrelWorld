#include "gentity.h"

//////////////////////////
// inline implementation
//////////////////////////
void GameEntity::SetRenderComp(RenderComponent *pRenderComp) 
{
	if(!m_spRenderComp.IsEmpty())
	{
		m_spRenderComp->SetParent(nullptr); // remove old parent
	}
	m_spRenderComp = pRenderComp;
	
	if(m_spRenderComp != nullptr)
	{
		m_spRenderComp->SetParent(this); // set new parent
		
		// set connectivity with other components
		if(m_spTransformComp != nullptr)
		{
			m_spTransformComp->Connect(*m_spRenderComp);
		}
	}
}

void GameEntity::SetAnimatorComp(AnimatorComponent* pAnimatorComp)
{
	m_spAnimatorComp = pAnimatorComp;
}

void GameEntity::SetTransformComp(TransformComponent* pTransComp)
{
	m_spTransformComp = pTransComp;
	
	// if transform is nullptr return 
	if(m_spTransformComp.IsEmpty()) return;
	
	// connect this component with other components - if it make sense
	if(m_spRenderComp != nullptr)
	{
		m_spTransformComp->Connect(*m_spRenderComp);
	}
	
}

void GameEntity::SetLogicComp(LogicComponentBase* pLogicComp)
{
	if(!m_spLogicComp.IsEmpty())
	{
		m_spLogicComp->SetParent(nullptr);
	}
	
	m_spLogicComp = pLogicComp;
	
	if(!m_spLogicComp.IsEmpty())
	{
		m_spLogicComp.In()->SetParent(this);
	}
	
}

void GameEntity::SetInputComp(InputComponent* pInputComp)
{
	m_spInputComp = pInputComp;
}
