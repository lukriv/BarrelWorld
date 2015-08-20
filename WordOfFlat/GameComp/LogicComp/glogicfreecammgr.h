#ifndef __GAME_LOGIC_FREE_CAMERA_MANAGER_H__
#define __GAME_LOGIC_FREE_CAMERA_MANAGER_H__


#include <GameComp/gmanagerbase.h>
#include "glogicfreecam.h"



class LogicFreeCameraManager : public GameManagerBase<LogicFreeCamera>
{
private:
	GameInputSystem* m_pInputSystem;
public:
	LogicFreeCameraManager(GameInputSystem* pInputSystem);
	~LogicFreeCameraManager();
	
	GameErrorCode CreateComponent(wxDword compId, LogicFreeCamera *&pNewComponent);

};

#endif // __GAME_LOGIC_FREE_CAMERA_MANAGER_H__
