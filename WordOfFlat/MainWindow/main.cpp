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

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
 
     // Load a sprite to display
     sf::Texture texture;
     if (!texture.loadFromFile("res/img/body.png"))
	 {
		 FWGLOG_ERROR_FORMAT(wxT("Loading resource file \"%s\" failed"), spLogger, wxT("res/img/body.png"));
		 return -1;
	 }
     sf::Sprite sprite(texture);
		 

	
      // Start the game loop
	while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
		 

        // Clear screen
        window.clear();
 
        // Draw the sprite
        window.draw(sprite);
 
        // Draw the string
        //App.Draw(Text);
		
		// Update the window
        window.display();
    }
	 
	return 0;
}
