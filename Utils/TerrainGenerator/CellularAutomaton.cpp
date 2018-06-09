#include "CellularAutomaton.h"

//#include <iostream>

#include <random>
#include <chrono>

#include <cstring>
//#include <cassert>
//#include "utils.h"


void CellularAutomatonMapGenerator::GenerateMap(HeightMap &map, const CellularAutomatonMapGenerator::Params& params)
{
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int32_t> distribution(0,99);

	size_t dataSize = map.GetSizeX() * map.GetSizeY();
	
	
	MapContainer<int8_t, SphereMapCoords> tempMap;
	tempMap.Initialize(map.GetSizeX(), map.GetSizeY());
	
	// initialize
	if(params.m_initialPercentage > 0)
	{
		for(uint8_t *pPointer = map.GetData(); pPointer < (map.GetData() + dataSize); ++pPointer)
		{
			if((*pPointer == params.m_growsOn)&&(distribution(generator) < params.m_initialPercentage))
			{
				*pPointer = params.m_lifeForm;
			}
		}
	}

	// copy to temporary map
	memcpy(tempMap.GetData(), map.GetData(), dataSize);
	
	int32_t count = 0;
	int8_t actual = 0;
	
	for(int32_t iter = 0; iter < params.m_iterations; ++iter)
	{
		
		//writeMap(pMap, mapSize);
		//std::cin >> count;
		
		for(int32_t y = 0; y < map.GetSizeY(); ++y)
		{
			for(int32_t x = 0; x < map.GetSizeX(); ++x)
			{
				actual = map.GetCellValue(x, y);
				
				// check if make sense compute something
				if((actual == params.m_lifeForm)||(actual == params.m_growsOn))
				{
				
					// count live neightbours
					int32_t nx, ny;
					count = 0;
					for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir)
					{
						map.GetNeightbourCoords((MapContainerDIR)dir, x, y, nx, ny);
						// for continent generator usage - higher is alive too
						if(map.GetCellValue(nx, ny) >= params.m_lifeForm) ++count;
					}
					
					if( actual == params.m_lifeForm)
					{
						if((params.m_aliveFrom > count)||(params.m_aliveTo < count))
						{
							// dies
							tempMap.SetCellValue(x,y,params.m_growsOn);
						}
					} else {
						if((params.m_bornFrom <= count)&&(params.m_bornTo >= count))
						{
							// goes alive
							tempMap.SetCellValue(x,y,params.m_lifeForm);
						}
					}
				}
			}
		}
		
		// copy to temporary map
		memcpy(map.GetData(), tempMap.GetData(), dataSize);
		
	}
	
	
}


