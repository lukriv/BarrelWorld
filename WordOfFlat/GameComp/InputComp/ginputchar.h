#ifndef __GAME_CHARACTER_INPUT_COMPONENT_H__01__
#define __GAME_CHARACTER_INPUT_COMPONENT_H__01__

#include "ginputcomp.h"
//#include <GameResHold/gdeftables.h>
#include <GameSystem/glog.h>

class GameInputSystem;

class CharacterInput : public InputComponent {
private:
	GameInputSystem *m_pInputSystem;
public:
	enum StateFlags {
		INPUT_ACTION_NONE =						0 ,
		INPUT_ACTION_LEFT = 				(1<<0),
		INPUT_ACTION_RIGHT = 				(1<<1),
		INPUT_ACTION_UP =					(1<<2),
		INPUT_ACTION_DOWN = 				(1<<3),
		INPUT_ACTION_FORWARD =				(1<<4),
		INPUT_ACTION_BACKWARD =				(1<<5),
		INPUT_ACTION_JUMP =					(1<<6),
		INPUT_ACTION_CROUCH =				(1<<7),
		INPUT_ACTION_ATTACK =	 			(1<<8),
		INPUT_ACTION_DEFEND =				(1<<9),
		INPUT_ACTION_USE =					(1<<10),
	};
	
public:
	CharacterInput();

	GameErrorCode Initialize( GameInputSystem *pInputSystem );

	GameErrorCode Create( const InputDef &inputDef);

	void SetMoveUp(bool move)
	{
		SetState(move, INPUT_ACTION_UP);
	}
	
	void SetMoveDown(bool move)
	{
		SetState(move, INPUT_ACTION_DOWN);
	}
	
	void SetMoveLeft(bool move)
	{
		SetState(move, INPUT_ACTION_LEFT);
	}
	
	void SetMoveRight(bool move)
	{
		SetState(move, INPUT_ACTION_RIGHT);
	}
	
	void SetMoveForward(bool move)
	{
		SetState(move, INPUT_ACTION_FORWARD);
	}
	
	void SetMoveBackward(bool move)
	{
		SetState(move, INPUT_ACTION_BACKWARD);
	}
};


#endif //__GAME_CHARACTER_INPUT_COMPONENT_H__01__