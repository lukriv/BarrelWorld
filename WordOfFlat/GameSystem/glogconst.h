#ifndef __GAME_LOG_CONSTANTS_H__
#define __GAME_LOG_CONSTANTS_H__

#include <wx/chartype.h>

struct GameLoggerRow{
	wxChar* m_loggerName;
	wxChar* m_logFileName;
};

static const GameLoggerRow DefaultLogger = {wxT("default"),wxT("default.log")};

static const GameLoggerRow GameLoggerTable[] = {
	{wxT("default"),wxT("default.log")}
};

static const wxDword GameLoggerTableSize = 1;

#endif