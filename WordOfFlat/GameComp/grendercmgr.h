#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include "gcomp.h"

class GameRenderComponentMgr : public IGameComponent {
	
// IGameComponent interface
public:
	virtual GameCompType GetType() {return GAME_COMP_RENDER;}
};


#endif //__GAME_RENDER_COMPONENT_H__