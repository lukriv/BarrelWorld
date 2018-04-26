#ifndef __SDL_UTILITIES_H__
#define __SDL_UTILITIES_H__

#include <SDL.h>
#include "ClimateCell.h"




class SDLutils
{
public:	
	enum class ACTION {
		NEXT,
		RELOAD,
		HEIGHT_MAP,
		CLIMATE_LEVEL_1,
		CLIMATE_LEVEL_2,
		CLIMATE_LEVEL_3,
		CLIMATE_LEVEL_4,
		CLIMATE_MOVE,
		CLIMATE_TEMPERATURE,
		CLIMATE_CLOUDS,
		CLIMATE_PRESSURE,
		CLIMATE_STEP,
		CLIMATE_FAST_STEP,
		MOUSE_CLICK,
		QUIT
	};
	
	enum ClimateFlag {
		NONE = 0,
		TEMPERATURE = 1,
		MOVE = 2,
		CLOUDS = 4,
		PRESSURE = 8
	};
	
	enum MoveFlag {
		HIGH_AIR = 1
	};
private:	
	//The window we'll be rendering to 
	SDL_Window* mWindow = nullptr; 
	
	SDL_Renderer* mRenderer = nullptr;
	
	//The surface contained by the window 
	SDL_Surface* mScreenSurface = nullptr; 
	
	int32_t mMultiplier;
	int32_t mLastX;
	int32_t mLastY;
	
public:
	SDLutils(int32_t SCREEN_WIDTH, int32_t SCREEN_HEIGHT, int32_t multiplier = 1);
	~SDLutils();
	
	void writeMap(const uint8_t *map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel);
	void writeClimateMap(const ClimateCell *map, int32_t mapSizeX, int32_t mapSizeY, int32_t level, int32_t flags );
	
	void getLastMousePos(int32_t &x, int32_t &y) { x = mLastX/mMultiplier; y = mLastY/mMultiplier; }
	
	ACTION waitForAction();
private:
	void writeMove(Urho3D::Vector2 &streamDir, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeMove(CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeType(CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeTemperature(CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writePressure(CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeClouds(AirContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
};

#endif // __SDL_UTILITIES_H__
