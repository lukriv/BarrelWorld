#ifndef __GAME_MESSAGE_CLIENT_H__
#define __GAME_MESSAGE_CLIENT_H__


class IGameMsgCli {
public:
	virtual GameErrorCode SendMsg() = 0;
	virtual GameErrorCode RegisterCallback() = 0;
	
}


#endif //__GAME_MESSAGE_SERVER_H__