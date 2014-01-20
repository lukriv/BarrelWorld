#ifndef __CONSOLE_GAME_ENGINE_H__
#define __CONSOLE_GAME_ENGINE_H__


#include "consoleWindow.h"

enum EKeyState {
	KEY_STATE_NONE 		= 0,
	KEY_STATE_LEFT 		= 1,
	KEY_STATE_RIGHT		= 1<<1,
	KEY_STATE_UP		= 1<<2,
	KEY_STATE_DOWN		= 1<<3,
	KEY_STATE_FIRE		= 1<<4
	
};


struct Character {
	ConsoleCoord pos;
};



class ConsoleGameEngine {
	ConsoleCoord m_viewFieldMin;
	ConsoleCoord m_viewFieldMax;
	Character m_avatar;
public:
	ConsoleGameEngine() 
	{
		m_viewFieldMin.x = m_viewFieldMin.y = 0;
		m_viewFieldMax.x = m_viewFieldMax.y = 0;
	}
	~ConsoleGameEngine() {}
	
	bool WriteMargin(ConsoleWindowWrapper &console, const ConsoleCoord &leftTopPos, const ConsoleCoord &insizeSize );
	bool Play(ConsoleWindowWrapper &console);
	
private:
	void SetUpFlag(unsigned int flag, bool set, unsigned int &flags)
	{
		if(set)
		{
			flags |= flag;
		} else {
			flags &= (~flag);
		}
	}	
	
};



#endif //__CONSOLE_GAME_ENGINE_H__
