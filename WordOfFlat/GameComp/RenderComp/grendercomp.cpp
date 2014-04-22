#include "grendercomp.h"
#include "grendercmgr.h"

RenderComponent::~RenderComponent()
{
	// destroy this
	m_pOwnerManager->DestroyRenderComponent(this);
}
