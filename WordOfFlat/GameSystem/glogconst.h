#ifndef __GAME_LOG_CONSTANTS_H__
#define __GAME_LOG_CONSTANTS_H__

#include <wx/chartype.h>

struct GameLoggerRow{
	GameLoggerRow(const wxChar* logName, const wxChar* logFileName):m_loggerName(logName), m_logFileName(logFileName){}
	const wxChar* m_loggerName;
	const wxChar* m_logFileName;
};

static const GameLoggerRow DefaultLogger = GameLoggerRow(wxT("default"), wxT("default.log"));

static const GameLoggerRow GameLoggerTable[] = {
	GameLoggerRow(wxT("default"),wxT("default.log")),
	GameLoggerRow(wxT("test"),wxT("test.log"))
};

static const wxDword GameLoggerTableSize = 2;

#endif