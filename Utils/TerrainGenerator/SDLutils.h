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
	
	int32_t mMultiplier;
	
public:
	SDLutils(int32_t SCREEN_WIDTH, int32_t SCREEN_HEIGHT, int32_t multiplier = 1);
	~SDLutils();
	
	void writeMap(const uint8_t *map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel);
	
	ACTION waitForAction();

};

#endif // __SDL_UTILITIES_H__
