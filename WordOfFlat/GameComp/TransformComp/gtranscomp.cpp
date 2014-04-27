#include "gtranscomp.h"


void TransformComponent::Connect(RenderComponent& renderComp)
{
	if(m_pSceneNode != NULL)
	{
		if(renderComp.GetOgreEntity() != NULL)
		{
			m_pSceneNode->attachObject(renderComp.GetOgreEntity());
		}
		
		if(renderComp.GetOgreCamera() != NULL)
		{
			m_pSceneNode->addChild(renderComp.GetOgreCamera()->GetCameraNode());
		}
		
		if(renderComp.GetOgreLight() != NULL)
		{
			m_pSceneNode->addChild(renderComp.GetOgreLight()->GetLightNode());
		}
	}
}
