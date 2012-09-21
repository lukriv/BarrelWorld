#include "../../extern/include/wx/setup.h"
#include "../../extern/include/wx/defs.h"
#include <stdio.h>

//#include "../../extern/include/wx/string.h"
//#include "../../extern/include/wx/file.h"
#include "../../extern/include/wx/app.h"
#include "../../extern/include/wx/log.h"

#include "../../extern/include/SFML/Graphics.hpp"



int main(int argc, char **argv)
{
	FILE *loggerFile = NULL;
	wxInitializer initializer;
	
    if ( !initializer )
    {
        printf("Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }
	
	// logger settings
	if (!(loggerFile = fopen("error.log", "w")))
	{
		printf("Failed to open logger file, aborting.");
        return -1;
	}
	
	wxLogStderr *logger = new (std::nothrow) wxLogStderr(loggerFile);
	delete wxLog::SetActiveTarget(logger);
	
	//set verbose logger (use for debug)
	wxLog::SetVerbose(true);
		
	wxLogInfo(_T("Infos are enabled"));
    wxLogWarning(_T("Warnings are enabled"));
	wxLogError(_T("Errors are enabled"));
	// Create the main window
	sf::RenderWindow App(sf::VideoMode(800, 600), "SFML window");
 
     // Load a sprite to display
     sf::Image Image;
     if (!Image.LoadFromFile("../../../Resources/images/body.png"))
	 {
		 wxLogError(_T("Body image load failed"));
		 return -1;
	 }
		 
     sf::Sprite Sprite(Image);
	
      // Start the game loop
	while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }
        // Clear screen
		App.Clear();
        
		// Draw the sprite
        App.Draw(Sprite);
 
        // Draw the string
        //App.Draw(Text);
 
        // Update the window
        App.Display();
    }
	 
	return 0;
}
