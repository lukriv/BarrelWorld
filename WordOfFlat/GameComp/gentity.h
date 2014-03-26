#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include "gtranscomp.h"
#include "grendercomp.h"
#include "ganimatormgr.h"


struct GameEntity {
	TransformComponent* m_pTransformComp;
	RenderComponent* m_renderComp;
	AnimatorComponent* m_animatorComp;

	GameEntity() {}
};

#endif // __GAME_ENTITY_H__
