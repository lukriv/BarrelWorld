#include <stdio.h>
#include <iostream>

#include <wx/init.h>
#include <wx/wxcrtvararg.h>
#include <wx/log.h>
//#include "../../extern/include/wx/string.h"
//#include "../../extern/include/wx/file.h"

#include "../GameSystem/gdefs.h"
#include "../GameSystem/glog.h"
#include "../GameSystem/refobject.h"
#include "../GameSystem/refobjectsmptr.h"

#include "gclientengine.h"

int main(int argc, char **argv)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxInitializer initializer;
	RefObjSmPtr<GameLogger> spLogger;
	
	
	wxInitializer intializer(argc, argv);
	if (!intializer.IsOk())
	{
		wxPrintf(wxT("Failed to initialize the wxWidgets library, aborting."));
		return -1;
	}
	
 	if (FWG_FAILED(GameLoggerCreator::CreateLogger(spLogger.OutRef(),wxT("default"))))
	{
		printf("Failed to initialize the logger, aborting.");
		return -1;
	}
	
	//FWGLOG_TRACE(wxT(" ***** Trace message ***** "), spLogger);
	//FWGLOG_DEBUG(wxT("  **** Debug message ****"), spLogger);
	FWGLOG_INFO(wxT("*************** GAME STARTED *****************"), spLogger);
	//FWGLOG_WARNING(wxT("    ** Warning message **"), spLogger);
	//FWGLOG_ERROR(wxT("     * Error message *"), spLogger);
	
	//FWGLOG_TRACE_FORMAT(wxT("Format: %s"), spLogger, wxT("Trace message"), FWGLOG_ENDVAL);
	//FWGLOG_DEBUG_FORMAT(wxT("Format: %s"), spLogger, wxT("Debug message"), FWGLOG_ENDVAL);
	//FWGLOG_INFO_FORMAT(wxT("Format: %s"), spLogger, wxT("Info message"), FWGLOG_ENDVAL);
	//FWGLOG_WARNING_FORMAT(wxT("Format: %s"), spLogger, wxT("Warning message"), FWGLOG_ENDVAL);
	//FWGLOG_ERROR_FORMAT(wxT("Format: %s"), spLogger, wxT("Error message"), FWGLOG_ENDVAL);


	GameClientEngine gameEngine;
	
	if (FWG_FAILED(result = gameEngine.Initialize(spLogger.In())))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateEngine() : Engine initialization failed: 0x%08x"),
					spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if (FWG_FAILED(result = gameEngine.CreateTestingWorld()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateEngine() : Create testing world failed: 0x%08x"),
					spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if (FWG_FAILED(result = gameEngine.MainLoop()))
	{
		FWGLOG_ERROR_FORMAT(wxT("GameClientEngine::CreateEngine() : Main loop failed: 0x%08x"),
					spLogger, result, FWGLOG_ENDVAL);
		return result;		
	}
	
	gameEngine.Uninitialize();
	 

	FWGLOG_INFO(wxT("*************** GAME END *****************"), spLogger);

	 
	return 0;
}
