#ifndef __CONSOLE_GAME_ENGINE_H__
#define __CONSOLE_GAME_ENGINE_H__


#include "consoleWindow.h"


class ConsoleGameEngine {
public:
	ConsoleGameEngine() {}
	~ConsoleGameEngine() {}
	
	bool WriteMargin(ConsoleWindowWrapper &console, const ConsoleCoord &leftTopPos, const ConsoleCoord &insizeSize );
	
};



#endif //__CONSOLE_GAME_ENGINE_H__
