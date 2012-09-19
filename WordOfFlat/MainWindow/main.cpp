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
	
	if (!(loggerFile = fopen("error.log", "a")))
	{
		printf("Failed to open logger file, aborting.");
        return -1;
	}
	
	wxLogStderr *logger = new (std::nothrow) wxLogStderr(loggerFile);
	delete wxLog::SetActiveTarget(logger);
	 
	wxLogMessage(_T("This is log message"));
    wxLogWarning(_T("This is log warning"));
	wxLogError(_T("This is error"));
	// Create the main window
	sf::RenderWindow App(sf::VideoMode(800, 600), "SFML window");
 
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
        //App.Draw(Sprite);
 
        // Draw the string
        //App.Draw(Text);
 
        // Update the window
        App.Display();
    }
	 
	return 0;
}
