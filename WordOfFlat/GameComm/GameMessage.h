#ifndef __GAME_MESSAGE_H__
#define __GAME_MESSAGE_H__

enum GameMessageType {
	GAME_MSG_TYPE_UNKNOWN = 0
};


class IGameMessage {
public:
	GameMessageType GetType() = 0;
	GameVersionType GetVersion() = 0;
	


	
}


#endif //__GAME_MESSAGE_H__