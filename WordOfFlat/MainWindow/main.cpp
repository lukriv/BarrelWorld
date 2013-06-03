#include <wx/setup.h>
#include <wx/defs.h>
#include <wx/app.h>

#include <stdio.h>
#include <iostream>

//#include "../../extern/include/wx/string.h"
//#include "../../extern/include/wx/file.h"

#include "../GameSystem/glog.h"
#include "../GameSystem/refobject.h"
#include "../GameSystem/refobjectsmptr.h"

#include <SFML/Graphics.hpp>


#include <wx/log.h>

int main(int argc, char **argv)
{
	
	wxInitializer initializer;
	RefObjSmPtr<GameLogger> spLogger;
	
    if ( !initializer )
    {
        printf("Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }
	
 	if (FWG_FAILED(GameLoggerCreator::CreateLogger(spLogger.OutRef(),wxT("default"))))
	{
		printf("Failed to initialize the logger, aborting.");
		return -1;
	}
	
	FWGLOG_TRACE(wxT("Trace message"), spLogger);
	FWGLOG_DEBUG(wxT("Debug message"), spLogger);
	FWGLOG_INFO(wxT("Info message"), spLogger);
	FWGLOG_WARNING(wxT("Warning message"), spLogger);
	FWGLOG_ERROR(wxT("Error message"), spLogger);
	
	FWGLOG_TRACE_FORMAT(wxT("Format: %s"), spLogger, wxT("Trace message"), FWGLOG_ENDVAL);
	FWGLOG_DEBUG_FORMAT(wxT("Format: %s"), spLogger, wxT("Debug message"), FWGLOG_ENDVAL);
	FWGLOG_INFO_FORMAT(wxT("Format: %s"), spLogger, wxT("Info message"), FWGLOG_ENDVAL);
	FWGLOG_WARNING_FORMAT(wxT("Format: %s"), spLogger, wxT("Warning message"), FWGLOG_ENDVAL);
	FWGLOG_ERROR_FORMAT(wxT("Format: %s"), spLogger, wxT("Error message"), FWGLOG_ENDVAL);


 
     // Load a sprite to display
     sf::Texture texture;
     if (!texture.loadFromFile("res/img/body.png"))
	 {
		 FWGLOG_ERROR_FORMAT(wxT("Loading resource file \"%s\" failed"), spLogger, wxT("res/img/body.png"));
		 return -1;
	 }
     sf::Sprite sprite(texture);
		 

	

	 
	return 0;
}
