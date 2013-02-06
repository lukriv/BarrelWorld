#ifndef __GAME_MESSAGE_SERVER_H__
#define __GAME_MESSAGE_SERVER_H__

#include "../GameMsgCli/gamemsgcli.h"

class IGameMsgSrv : public IGameMsgCli, public IRefObject{
public:
	GameErrorCode CreateLocalMsgCli(IGameMsgCli *& pMsgCli)	= 0;
	
}


#endif //__GAME_MESSAGE_SERVER_H__