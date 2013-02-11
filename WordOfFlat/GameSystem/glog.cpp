#include "glog.h"
#include <cstdio>

GameErrorCode GameLogger::CreateLogger()
{
	FILE* pLoggerFile = NULL;
	GameErrorCode result = FWG_NO_ERROR;
	
	if (!(pLoggerFile = fopen("error.log", "w")))
	{
		printf("Failed to open logger file, aborting.");
        return -1;
	}
		
	wxLogStderr *stdLogger = new (std::nothrow) wxLogStderr(pLoggerFile);
	if(!stdLogger)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	delete wxLog::SetActiveTarget(stdLogger);
	
	return result;

}






