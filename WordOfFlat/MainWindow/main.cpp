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
	
	if (FWG_FAILED(GameLogger::CreateLogger()))
	{
		printf("Failed to initialize the logger, aborting.");
		return -1;
	}

	wxLogMessage(wxT("Infos are enabled"));
	wxLogWarning(wxT("Warnings are enabled"));
	wxLogError(wxT("Errors are enabled"));

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
 
     // Load a sprite to display
     sf::Texture texture;
     if (!texture.loadFromFile("res/img/body.png"))
	 {
		 wxLogError(_T("Loading resource file \"%s\" failed"), wxT("res/img/body.png"));
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
