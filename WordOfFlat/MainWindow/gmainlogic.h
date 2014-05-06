#ifndef __GAME_MAIN_LOGIC_H__
#define __GAME_MAIN_LOGIC_H__

class GameMainLogicBase : public IRefObject {
public:
	
	virtual GameErrorCode InitializeCameras() = 0;
	virtual GameErrorCode InitializeMenus() = 0;
	virtual GameErrorCode InitializeLights() = 0;
	virtual GameErrorCode InitializeInputs() = 0;
	
	virtual ~GameMainLogicBase();

};

#endif // __GAME_MAIN_LOGIC_H__
