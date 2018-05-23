
#include "CellularAutomaton.h"
#include "ContinentGenerator.h"

#include <stdio.h>
#include <cctype>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include "utils.h"
#include "SDLutils.h"
#include "global.h"
#include "debugConsole.h"
#include "ClimateCell.h"
#include "ClimateGenerator.h"

#define LINE_SIZE 256

std::ostream *gErrOut = nullptr;

struct UniversalParam {
	enum ParamType {
		UNDEF,
		CELL,
		CONT
	};
	
	ParamType m_type;
	
	CellularAutomatonMapGenerator::Params m_cellParams;
	ContinentMapGenerator::Params m_contParams;
	
	UniversalParam() : m_type(UNDEF) {}
};



void clearParams(UniversalParam &params)
{
	params.m_type = UniversalParam::UNDEF;
	params.m_cellParams.clearParams();
	params.m_contParams.clearParams();
}

void parseLineParam(const std::string& line, std::string& name, int32_t& value)
{
	uint32_t iter = 0;
	std::string valueStr;
	name.clear();
	value = -1;
	
	while((iter < line.size())&&(line[iter] != '='))
	{
		if(!isspace(line[iter]))
		{
			name.append(1,line[iter]);
		}
		++iter;
	}
	
	++iter;
	
	if(iter >= line.size())
	{
		return;
	}
	
	while(iter < line.size())
	{
		if(isdigit(line[iter]))
		{
			valueStr.append(1, line[iter]);
		}
		++iter;
	}
	
	value = std::stoi(valueStr, nullptr);
	
}

bool readParam(std::istream &input, CellularAutomatonMapGenerator::Params &params)
{
	char line[LINE_SIZE];
	
	std::string name;
	int32_t value;
	
	while(!input.eof())
	{
		memset(line, 0, LINE_SIZE);
		input.getline(line, LINE_SIZE);
		parseLineParam(std::string(line), name, value);
		if(name.compare("growsOn") == 0){
			params.m_growsOn = value;
		} else if(name.compare("lifeForm") == 0) {
			params.m_lifeForm = value;
		} else if(name.compare("initialPercentage") == 0) {
			params.m_initialPercentage = value;
		} else if(name.compare("bornFrom") == 0) {
			params.m_bornFrom = value;
		} else if(name.compare("bornTo") == 0) {
			params.m_bornTo = value;
		} else if(name.compare("aliveFrom") == 0) {
			params.m_aliveFrom = value;
		} else if(name.compare("aliveTo") == 0) {
			params.m_aliveTo = value;
		} else if(name.compare("iterations") == 0) {
			params.m_iterations = value;
		} else if(name.compare("END") == 0) {
			break;
		} else {
			*gErrOut << "ERROR: reading CellularAutomatonMapGenerator at params.txt failed" << std::endl;
			*gErrOut << "Failed parameter: '" << name << "'" << std::endl;
			return false;
		}
		
	}
	return true;
}

bool readParam(std::istream &input, ContinentMapGenerator::Params &params)
{
	char line[LINE_SIZE];
	
	std::string name;
	int32_t value;
	
	while(!input.eof())
	{
		memset(line, 0, LINE_SIZE);
		input.getline(line, LINE_SIZE);
		parseLineParam(std::string(line), name, value);
		if(name.compare("waterLevel") == 0){
			params.m_waterLevel = value;
		} else if(name.compare("groundPercentage") == 0) {
			params.m_groundPercentage = value;
		} else if(name.compare("jitterPercentage") == 0) {
			params.m_jitterPercentage = value;
		} else if(name.compare("jitterStrong") == 0) {
			params.m_jitterStrong = value;
		} else if(name.compare("chunkSizeMin") == 0) {
			params.m_chunkSizeMin = value;
		} else if(name.compare("chunkSizeMax") == 0) {
			params.m_chunkSizeMax = value;
		} else if(name.compare("compactGround") == 0) {
			params.m_compactGround = value; 
		} else if(name.compare("highRiseProb") == 0) {
			params.m_highRiseProb = value;
		} else if(name.compare("sinkProb") == 0) {
			params.m_sinkProb = value; 
		} else if(name.compare("minElev") == 0) {
			params.m_minElev = value;
		} else if(name.compare("maxElev") == 0) {
			params.m_maxElev = value; 
		} else if(name.compare("regions") == 0) {
			params.m_regions = value;
		} else if(name.compare("xBorder") == 0) {
			params.m_xBorder = value; 			
		} else if(name.compare("yBorder") == 0) {
			params.m_yBorder = value; 			
		} else if(name.compare("erosion") == 0) {
			params.m_erosion = value;
		} else if(name.compare("addMode") == 0) {
			params.m_addMode = value;
		} else if(name.compare("END") == 0) {
			break;
		} else {
			*gErrOut << "ERROR: reading ContinentMapGenerator at params.txt failed" << std::endl;
			*gErrOut << "Failed parameter: '" << name << "'" << std::endl; 
			return false;
		}
		
	}
	return true;
}


bool readParams(int8_t &initialValue, int32_t &mapSizeX, int32_t &mapSizeY, int32_t &multiplier, std::vector<UniversalParam> &paramList)
{
	char line[LINE_SIZE];
	std::ifstream ifile("params.txt");
	
	if(ifile.fail())
	{
		*gErrOut << "ERROR: Opening params file failed" << std::endl;
		return false;
	}
	
	std::string name;
	int32_t value;
	
	UniversalParam params;
	
	while(!ifile.eof())
	{
		memset(line, 0, LINE_SIZE);
		ifile.getline(line, LINE_SIZE);
		parseLineParam(std::string(line), name, value);
		if(name.compare("CELL_PARAM") == 0) {
			
			
			if(!readParam(ifile, params.m_cellParams))
			{
				return false;
			}
			
			params.m_type = UniversalParam::CELL;
			
			paramList.push_back(params);
			clearParams(params);
		} else if(name.compare("CONT_PARAM") == 0) {
			
			if(!readParam(ifile, params.m_contParams))
			{
				return false;
			}
			
			params.m_type = UniversalParam::CONT;
			
			paramList.push_back(params);
			clearParams(params);
		} else if(name.compare("BREAK") == 0) {
			break;
		} else if(name.compare("BASE") == 0) {
			initialValue = value;
		} else if(name.compare("MAPSIZEX") == 0) {
			mapSizeX = value;
		} else if(name.compare("MAPSIZEY") == 0) {
			mapSizeY = value;
		} else if(name.compare("MULTIPLIER") == 0) {
			multiplier = value;
		} else {
			*gErrOut << "ERROR: reading params.txt failed" << std::endl;
			*gErrOut << "Failed parameter: '" << name << "'" << std::endl;
			return false;
		}
		
	}

	
	return true;
}


void writeClimateMoveSelection(int32_t climateMoveLevel, bool force)
{
	std::string str;
	switch (climateMoveLevel)
	{
		case 0:
			str = "water";
			break;
		case 1:
			str = "low";
			break;
		case 2:
			str = "high";
			break;
	}

	std::cout << "Display move: " << str << ", ";
	str = (force)?"force":"wind/stream";
	std::cout << str << std::endl;
	
}


static const int32_t MAP_SIZE = 65;

SDLutils *gSDL = nullptr;

int WinMain(int argc, char **argv)
{
	std::vector<UniversalParam> paramsList;
	MapContainer<uint8_t, SphereMapCoords> map;
	MapContainer<ClimateCell, SphereMapCoords> climateMap;
	
	std::ofstream file("output.txt");
	InitializeDebugConsole();
	gErrOut = &file;
	//std::ostream& file = std::cerr;
	int8_t initialVal = 0;
	int32_t mapSizeX = MAP_SIZE*2;
	int32_t mapSizeY = MAP_SIZE;
	int32_t multiplier = 1;
	if(!readParams(initialVal, mapSizeX, mapSizeY, multiplier, paramsList))
	{
		*gErrOut << "Read params failed" << std::endl;
		return -1;
	}
	gErrOut = &std::cout;
	gSDL = new SDLutils(mapSizeX, mapSizeY, multiplier);
	
	map.Initialize(mapSizeX, mapSizeY);
	
	int32_t mapx,mapy;
	bool quit = false;
	while (!quit)
	{
		*gErrOut << " ---Start--- " << std::endl;
		if(paramsList.empty())
		{
			*gErrOut << "Parameter list is empty" << std::endl;
			return -1;
		}
		
		//if((paramsList.size() == 1)&&(paramsList[0].m_type == UniversalParam::CONT))
		//{
		//	UniversalParam params;
		//	params.m_type = UniversalParam::CELL;
		//	params.m_cellParams.m_aliveFrom = 4;
		//	params.m_cellParams.m_aliveTo = 8;
		//	params.m_cellParams.m_bornFrom = 6;
		//	params.m_cellParams.m_bornTo = 8;
		//	params.m_cellParams.m_iterations = 10;
		//	params.m_cellParams.m_initialPercentage = 0;
		//	
		//	for(int8_t level = paramsList[0].m_contParams.m_minElev; level < paramsList[0].m_contParams.m_maxElev; ++level)
		//	{
		//		params.m_cellParams.m_growsOn = level;
		//		params.m_cellParams.m_lifeForm = level + 1;
		//		paramsList.push_back(params);
		//	}
		//}
		
		map.FillMap(initialVal);
		int32_t waterLevel = 1;
		
		for(auto &par: paramsList)
		{
			
			switch(par.m_type)
			{
				case UniversalParam::CELL :
					CellularAutomatonMapGenerator::GenerateMap(map, par.m_cellParams);
					break;
				case UniversalParam::CONT :
					waterLevel = par.m_contParams.m_waterLevel;
					
					ContinentMapGenerator::GenerateMap(map, par.m_contParams);
					//gSDL->writeMap( map.GetData(), map.GetSizeX(), map.GetSizeY(), waterLevel);
					//gSDL->waitForAction();
					//ContinentMapGenerator::ErodeMap(map, par.m_contParams);
					
					break;
				case UniversalParam::UNDEF :
					*gErrOut << "Parameter has unknown type" << std::endl;
					return -1;
			}
			
			ClimateGenerator climate(map, climateMap, waterLevel);
			
			gSDL->writeMap( map.GetData(), map.GetSizeX(), map.GetSizeY(), waterLevel);
			
			writeMapStatistics(*gErrOut, map.GetData(), map.GetSizeX(), map.GetSizeY(), waterLevel);		
		
			SDLutils::ACTION act = gSDL->waitForAction();
			
			int32_t climateLevel = -1;
			bool climateMove = false;
			int32_t climateMoveHigh = 0;
			bool climateMoveForce = false;
			bool climateTemp = false;
			bool climateClouds = false;
			bool climatePressure = false;
			bool climateVolume = false;
			bool climateSunPosition = false;
			
			while((act == SDLutils::ACTION::HEIGHT_MAP)
				||(act == SDLutils::ACTION::CLIMATE_LEVEL_1)
				||(act == SDLutils::ACTION::CLIMATE_LEVEL_2)
				||(act == SDLutils::ACTION::CLIMATE_LEVEL_3)
				||(act == SDLutils::ACTION::CLIMATE_FORCE)
				||(act == SDLutils::ACTION::CLIMATE_TEMPERATURE)
				||(act == SDLutils::ACTION::CLIMATE_MOVE_HIGH)
				||(act == SDLutils::ACTION::CLIMATE_MOVE)
				||(act == SDLutils::ACTION::CLIMATE_CLOUDS)
				||(act == SDLutils::ACTION::CLIMATE_PRESSURE)
				||(act == SDLutils::ACTION::CLIMATE_VOLUME)
				||(act == SDLutils::ACTION::CLIMATE_SUN_POSITION)
				||(act == SDLutils::ACTION::CLIMATE_STEP)
				||(act == SDLutils::ACTION::CLIMATE_FAST_STEP)
				||(act == SDLutils::ACTION::MOUSE_CLICK)
				||(act == SDLutils::ACTION::WHEEL_DOWN)
				||(act == SDLutils::ACTION::WHEEL_UP)
				||(act == SDLutils::ACTION::UP)
				||(act == SDLutils::ACTION::DOWN)
				||(act == SDLutils::ACTION::LEFT)
				||(act == SDLutils::ACTION::RIGHT))
			{
				switch(act)
				{
					case SDLutils::ACTION::HEIGHT_MAP:
						climateLevel = -1;
						break;
					case SDLutils::ACTION::CLIMATE_LEVEL_1:
						climateLevel = 0;
						break;
					case SDLutils::ACTION::CLIMATE_LEVEL_2:
						climateLevel = 1;
						break;
					case SDLutils::ACTION::CLIMATE_LEVEL_3:
						climateLevel = 2;
						break;
					case SDLutils::ACTION::CLIMATE_FORCE:
						//climateMoveForce = !climateMoveForce; - no force change
						if(climateMove)
						{
							writeClimateMoveSelection(climateMoveHigh, climateMoveForce);
						}
						break;
					case SDLutils::ACTION::CLIMATE_TEMPERATURE:
						climateTemp = !climateTemp;
						break;
					case SDLutils::ACTION::CLIMATE_MOVE_HIGH:
						climateMoveHigh = (climateMoveHigh + 1) % 2;
						if(climateMove)
						{
							writeClimateMoveSelection(climateMoveHigh, climateMoveForce);
						}
						break;
					case SDLutils::ACTION::CLIMATE_MOVE:
						climateMove = !climateMove;
						if(climateMove)
						{
							writeClimateMoveSelection(climateMoveHigh, climateMoveForce);
						} else {
							std::cout << "Disable move display" << std::endl;
						}
						break;
					case SDLutils::ACTION::CLIMATE_CLOUDS:
						climateClouds = !climateClouds;
						break;
					case SDLutils::ACTION::CLIMATE_PRESSURE:
						climatePressure = !climatePressure;
						break;
					case SDLutils::ACTION::CLIMATE_VOLUME:
						climateVolume = !climateVolume;
						break;
					case SDLutils::ACTION::CLIMATE_SUN_POSITION:
						climateSunPosition = !climateSunPosition;
						break;
					case SDLutils::ACTION::CLIMATE_FAST_STEP:
						try {
							for(int32_t r = 0; r < 100; ++r)
								climate.SimulateClimateStep();
							
						} catch (ClimateGenerator::ClimateException e) {
							std::cout << "Exception: " << e.what() << std::endl;
						}
					case SDLutils::ACTION::CLIMATE_STEP:
						try {
							climate.SimulateClimateStep();
							std::cout << " -- Climate simulation End --" << std::endl;
							writeClimateStatistics(*gErrOut, climateMap, waterLevel);
						} catch (ClimateGenerator::ClimateException e) {
							std::cout << "Exception: " << e.what() << std::endl;
						}
						break;
					case SDLutils::ACTION::MOUSE_CLICK:
						{
							int32_t x,y;
							gSDL->getLastMousePos(x,y);
							writeCellStats(*gErrOut, climateMap, x, y, climateLevel);
						}
						break;
					case SDLutils::ACTION::WHEEL_DOWN:
						gSDL->updateMultiplier(-1);
						break;
					case SDLutils::ACTION::WHEEL_UP:
						gSDL->updateMultiplier(1);
						break;
					case SDLutils::ACTION::UP:
						gSDL->updateScreenBegin(0,-1);
						break;
					case SDLutils::ACTION::DOWN:
						gSDL->updateScreenBegin(0,1);
						break;
					case SDLutils::ACTION::LEFT:
						gSDL->updateScreenBegin(-1,0);
						break;
					case SDLutils::ACTION::RIGHT:
						gSDL->updateScreenBegin(1, 0);
						break;
					default:
						break;
				}
				if(climateLevel < 0 )
				{
					gSDL->writeMap( map.GetData(), map.GetSizeX(), map.GetSizeY(), waterLevel);
				} else {
					int32_t flags = 0;
					flags |= (climateTemp)? SDLutils::TEMPERATURE : 0;
					//flags |= (climateMove)? SDLutils::MOVE : 0;
					//flags |= (climateClouds)? SDLutils::CLOUDS : 0;
					flags |= (climatePressure)? SDLutils::PRESSURE : 0;
					flags |= (climateVolume)? SDLutils::VOLUME : 0;
									
					gSDL->writeClimateMap(climateMap.GetData(), climateMap.GetSizeX(), climateMap.GetSizeY(), climateLevel, flags);
				}
				
				if(climateClouds)
				{
					gSDL->writeClouds(climateMap.GetData(), climateMap.GetSizeX(), climateMap.GetSizeY());
				}
				
				if(climateMove)
				{
					int32_t flags = 0;
					flags |= (climateMoveForce)?SDLutils::MOVE_FORCE : 0;
					flags |= (climateMoveHigh == 2)?SDLutils::MOVE_HIGH : (climateMoveHigh == 0)? SDLutils::MOVE_WATER : 0;
					gSDL->writeMove(climateMap.GetData(), climateMap.GetSizeX(), climateMap.GetSizeY(), flags);
				}
				
				if(climateSunPosition)
				{
					int32_t x, y;
					climate.GetSunPosition(x,y);
					gSDL->writeSun(x,y);
				}
				
				act = gSDL->waitForAction();
			}
			
			if(act == SDLutils::ACTION::RELOAD)
			{
				break;
			}
			
			if(act == SDLutils::ACTION::QUIT)
			{
				quit = true;
				break;
			}
			
			
			
		}
		
		if(!quit)
		{
			paramsList.clear();
			if(!readParams(initialVal, mapx, mapy, multiplier, paramsList))
			{
				*gErrOut << "Read params failed" << std::endl;
				return -1;
			}
			
			if((mapx!=mapSizeX)||(mapy!=mapSizeY))
			{
				*gErrOut << "Mapsize cannot be changed during run" << std::endl;
				return -1;
			}
		}
	
	}
	
	delete gSDL;
	
	ShutdownDebugConsole();
	
	return 0;
}
