#ifndef __GAME_LOGIC_FREE_CAMERA_MANAGER_H__
#define __GAME_LOGIC_FREE_CAMERA_MANAGER_H__


#include <GameComp/gmanagerbase.h>
#include "glogicfreecam.h"



class LogicFreeCameraManager : public GameManagerBase<LogicFreeCamera>
{
public:
	LogicFreeCameraManager(GameLogger *pLogger);
	~LogicFreeCameraManager();
	
	GameErrorCode CreateComponent(wxDword compId, LogicFreeCamera *&pNewComponent);

};

#endif // __GAME_LOGIC_FREE_CAMERA_MANAGER_H__
