#ifndef __GAME_FREE_CAMERA_INPUT_COMPONENT_H__01__
#define __GAME_FREE_CAMERA_INPUT_COMPONENT_H__01__

#include "ginputcomp.h"
#include <GameResHold/gdeftables.h>
#include <GameSystem/glog.h>

class GameInputSystem;

class GenericMouseInput : public InputComponent {
public:
	static GameErrorCode CreateObject( const InputDef &inputDef, GameInputSystem& inputSystem, InputComponent *&pNewInputComp);

};


#endif //__GAME_FREE_CAMERA_INPUT_COMPONENT_H__01__