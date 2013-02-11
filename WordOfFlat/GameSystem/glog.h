#ifndef __GAME_LOG_H__
#define __GAME_LOG_H__


#include <wx/log.h>
#include <wx/atomic.h>
#include "gerror.h"
#include "refobject.h"

class GameLogger : public IRefObject {
public:
	static GameErrorCode CreateLogger();
	
};



#endif //__GAME_LOG_H__