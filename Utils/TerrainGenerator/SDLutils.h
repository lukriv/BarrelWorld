#ifndef __SDL_UTILITIES_H__
#define __SDL_UTILITIES_H__

#include <SDL.h>




class SDLutils
{
public:	
	enum class ACTION {
		NEXT,
		RELOAD,
		QUIT
	};
private:	
	//The window we'll be rendering to 
	SDL_Window* mWindow = nullptr; 
	
	//The surface contained by the window 
	SDL_Surface* mScreenSurface = nullptr; 
	
public:
	SDLutils(int32_t SCREEN_WIDTH, int32_t SCREEN_HEIGHT);
	~SDLutils();
	
	void writeMap(const int8_t *map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel);
	
	ACTION waitForAction();

};

#endif // __SDL_UTILITIES_H__
