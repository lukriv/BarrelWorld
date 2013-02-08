#include <wx/setup.h>
#include <wx/defs.h>
#include <wx/app.h>
#include <stdio.h>

//#include "../../extern/include/wx/string.h"
//#include "../../extern/include/wx/file.h"

#include "../GameSystem/glog.h"
#include "../GameSystem/refobject.h"
#include "../GameSystem/refobjectsmptr.h"

#include <SFML/Graphics.hpp>



int main(int argc, char **argv)
{
	
	wxInitializer initializer;
	RefObjSmPtr<GameLogger> spLogger;
	
    if ( !initializer )
    {
        printf("Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }
	
	if (FWG_FAILED(GameLogger::CreateLogger(spLogger.OutRef())))
	{
		printf("Failed to initialize the logger, aborting.");
		return -1;
	}
	
	spLogger->GetLogger()->LogRecord(wxLOG_Status, wxString(wxT("Status logs are enabled")),
		wxLogRecordInfo(__FILE__, __LINE__, __FUNCTION__, NULL));
		
	spLogger->GetLogger()->LogRecord(wxLOG_Message , wxString(wxT("Infos are enabled")),
		wxLogRecordInfo(__FILE__, __LINE__, __FUNCTION__, NULL));
    spLogger->GetLogger()->LogRecord(wxLOG_Warning, wxString(wxT("Warnings are enabled")),
		wxLogRecordInfo(__FILE__, __LINE__, __FUNCTION__, NULL));
	spLogger->GetLogger()->LogRecord(wxLOG_Error, wxString(wxT("Errors are enabled")),
		wxLogRecordInfo(__FILE__, __LINE__, __FUNCTION__, NULL));
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
 
     // Load a sprite to display
     sf::Texture texture;
     if (!texture.loadFromFile("res/img/body.png"))
	 {
		 wxLogError(_T("Loading file error"));
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
