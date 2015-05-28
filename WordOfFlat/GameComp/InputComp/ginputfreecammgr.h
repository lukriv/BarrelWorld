#ifndef __GAME_INPUT_FREE_CAMERA_H__
#define __GAME_INPUT_FREE_CAMERA_H__


#include "ginputfreecam.h"
#include <GameSystem/gerror.h>
#include <GameComp/gmanagerbase.h>
class GameInputSystem;


class InputFreeCamManager : public GameManagerBase<FreeCameraInput>
{
private:
	GameInputSystem* m_pInputSystem;
public:
	InputFreeCamManager(GameInputSystem* pInputSystem);
	~InputFreeCamManager();
	
	GameErrorCode CreateComponent( wxDword compId,  FreeCameraInput *&pNewInputComp);

};

#endif // __GAME_INPUT_FREE_CAMERA_H__
