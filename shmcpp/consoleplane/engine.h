#ifndef __CONSOLE_GAME_ENGINE_H__
#define __CONSOLE_GAME_ENGINE_H__


#include "consoleWindow.h"

struct Character {
	ConsoleCoord pos;
};


class ConsoleGameEngine {
	ConsoleCoord m_viewFieldMin;
	ConsoleCoord m_viewFieldMax;
	Character m_avatar;
public:
	ConsoleGameEngine(): m_viewFieldMin(0,0)
						,m_viewFieldMax(0,0) {}
	~ConsoleGameEngine() {}
	
	bool WriteMargin(ConsoleWindowWrapper &console, const ConsoleCoord &leftTopPos, const ConsoleCoord &insizeSize );
	bool Play();
	
	
	
};



#endif //__CONSOLE_GAME_ENGINE_H__
