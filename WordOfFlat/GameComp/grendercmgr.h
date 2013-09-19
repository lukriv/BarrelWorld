#ifndef __GAME_RENDER_COMPONENT_MANAGER_H__
#define __GAME_RENDER_COMPONENT_MANAGER_H__

#include "gcompmgrimpl.h"

class GameRenderComponentMgr : public GameComponentMgrBase {
	
// IGameComponent interface
public:
	virtual GameCompMgrType GetType() {return GAME_COMP_RENDER;}
	
	GameErrorCode RenderAll(){}
	
};


#endif //__GAME_RENDER_COMPONENT_H__