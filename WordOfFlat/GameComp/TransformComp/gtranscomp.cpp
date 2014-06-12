#include "gtranscomp.h"


void TransformComponent::Connect(RenderComponent& renderComp)
{
	if(m_pSceneNode != NULL)
	{
		renderComp.ConnectTransformComp(*this);
	}
}
