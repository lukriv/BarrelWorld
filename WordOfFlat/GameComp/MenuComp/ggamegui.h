#ifndef __GAME_GUI_H__
#define __GAME_GUI_H__

#include "gmenubase.h"


class GameGui : public GameMenuBase
{
public:
	GameGui(GameMenuSystem *pMenuRes, GameInputSystem *pInputSystem, GameLogger* pLogger);
	~GameGui();

};

#endif // __GAME_GUI_H__
