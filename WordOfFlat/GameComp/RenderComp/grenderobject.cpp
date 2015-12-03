#include "grenderobject.h"

RenderObject::RenderObject(RenderSystem* pCompManager) : RenderComponentBase(GAME_COMP_RENDER_OBJECT, pCompManager)
{
}

RenderObject::~RenderObject()
{
}

GameErrorCode RenderObject::Initialize(RenderPosition* pRenderMoveable)
{
	m_spPosition = pRenderPosition;
    if(m_spPosition.IsEmpty()||(pRenderPosition->GetSceneNode() == nullptr)) 
	{
		// transform component cannot be null - add transform component to entity at first
		m_spPosition.Release();
		return FWG_E_INVALID_PARAMETER_ERROR;
    }

	return FWG_NO_ERROR;
}
