#ifndef __GAME_LOG_H__
#define __GAME_LOG_H__


#include <wx/log.h>
#include "gerror.h"

#define FWGLOG_DETAIL(message)
#define FWGLOG_DEBUG(message)
#define FWGLOG_INFO(message)
#define FWGLOG_WARNING(message)
#define FWGLOG_ERROR(message)

#define FWGLOG_DETAIL_FORMAT(messageFormat,...)
#define FWGLOG_DEBUG_FORMAT(messageFormat,...)
#define FWGLOG_INFO_FORMAT(messageFormat,...)
#define FWGLOG_WARNING_FORMAT(messageFormat,...)
#define FWGLOG_ERROR_FORMAT(messageFormat,...)


class LogFormatterWithThread : public wxLogFormatter
{
	virtual wxString Format(wxLogLevel level,
	const wxString& msg,
	const wxLogRecordInfo& info) const
	{
	return wxString::Format("[%d] %s(%d) : %s",
	info.threadId, info.filename, info.line, msg);
	}
};


class GameLogger {
public:
	static GameErrorCode CreateLogger();
	
	static void LogDetail(const wxString& msg, const wxLogRecordInfo& info);
	static void LogDebug(const wxString& msg, const wxLogRecordInfo& info);
	static void LogInfo(const wxString& msg, const wxLogRecordInfo& info);
	static void LogWarning(const wxString& msg, const wxLogRecordInfo& info);
	static void LogError(const wxString& msg, const wxLogRecordInfo& info);
	
	static void LogDetailFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	static void LogDebugFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	static void LogInfoFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	static void LogWarningFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	static void LogErrorFormat(const wxString& formatStr, const wxLogRecordInfo& info, ...);
	
};



#endif //__GAME_LOG_H__