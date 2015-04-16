#ifndef __GAME_RENDER_TEST_BOX_H__
#define __GAME_RENDER_TEST_BOX_H__


#include "grendercomp.h"


class RenderTestBox : public RenderComponent
{
	
	RefObjSmPtr<TransformComponent> m_spTransform;
public:
	RenderTestBox();
	~RenderTestBox();
	
	
	GameErrorCode Initialize(TransformComponent *pTransform);

};

#endif // __GAME_RENDER_TEST_BOX_H__
