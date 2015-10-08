#ifndef __GAME_INPUT_COMPONENT_MANAGER_H__
#define __GAME_INPUT_COMPONENT_MANAGER_H__

#include "ginputchar.h"
#include "gmanagerbase.h"


class InputComponentManager : public GameManagerBase<InputComponent>
{
public:
	InputComponentManager(GameInputSystem* pInputSystem);
	~InputComponentManager();
};

#endif // __GAME_INPUT_COMPONENT_MANAGER_H__
