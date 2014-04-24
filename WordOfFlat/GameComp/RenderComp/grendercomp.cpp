#include "grendercomp.h"
#include "grendercmgr.h"

RenderComponent::~RenderComponent()
{
	Clear();
}


void RenderComponent::Clear()
{
	if(m_pOgreObject != NULL)
	{
		m_pOwnerManager->GetOgreSceneManager()->destroyMovableObject(m_pOgreObject);
		m_pOgreObject = NULL;
		m_compType = RENDER_COMP_UNDEFINED;
	}
}