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
		CLIMATE_MOVE_HIGH,
		CLIMATE_MOVE,
		CLIMATE_TEMPERATURE,
		CLIMATE_CLOUDS,
		CLIMATE_MOISTURE,
		CLIMATE_PRESSURE,
		CLIMATE_VOLUME,
		CLIMATE_STEP,
		CLIMATE_FAST_STEP,
		CLIMATE_SUN_POSITION,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		WHEEL_DOWN,
		WHEEL_UP,
		MOUSE_CLICK,
		LOAD_MAPS,
		STORE_MAPS,
		QUIT
	};
	
	enum ClimateFlag {
		NONE = 0,
		TEMPERATURE = 1,
		//MOVE = 2,
		//CLOUDS = 4,
		PRESSURE = 8,
		VOLUME = 16
	};
	
	enum MoveFlag {
		MOVE_HIGH = 1,
		MOVE_FORCE = 2,
		MOVE_WATER = 4
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
	
	int32_t mScreenBeginX;
	int32_t mScreenBeginY;
	
	int32_t mMapWidth;
	int32_t mMapHeight;
	
public:
	SDLutils(int32_t MAP_WIDTH, int32_t MAP_HEIGHT, int32_t multiplier = 1);
	~SDLutils();
	
	void updateMultiplier(int32_t update);
	
	void updateScreenBegin(int32_t xDiff, int32_t yDiff);
	
	void writeMap(const uint8_t *map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel);
	void writeClimateMap(const ClimateCell *map, int32_t mapSizeX, int32_t mapSizeY, int32_t level, int32_t flags );
	
	void writeClouds(const ClimateCell *map, int32_t mapSizeX, int32_t mapSizeY);
	
	void writeMoisture(const ClimateCell *map, int32_t mapSizeX, int32_t mapSizeY);
	
	void writeMove(const ClimateCell *map, int32_t mapSizeX, int32_t mapSizeY, int32_t flags );
	
	void writeSun(int32_t x, int32_t y);
	
	void getLastMousePos(int32_t &x, int32_t &y) { x = (mLastX/mMultiplier) + mScreenBeginX; y = (mLastY/mMultiplier) + mScreenBeginY; }
	
	ACTION waitForAction();
private:
	void writeMove(const Urho3D::Vector2 &streamDir, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeMove(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeType(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeTemperature(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writePressure(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeVolume(const CellContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeClouds(const AirContent* cont, int32_t xPos, int32_t yPos, int32_t flags = 0);
	void writeMoisture(const ClimateCell& cont, int32_t xPos, int32_t yPos, int32_t flags);
};

#endif // __SDL_UTILITIES_H__
