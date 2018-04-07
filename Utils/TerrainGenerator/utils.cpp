#include "utils.h"

void writeMap(const int8_t *map, int32_t mapSizeX, int32_t mapSizeY)
{
	writeMap(std::cout, map, mapSizeX, mapSizeY);
}

void writeMap(std::ostream& out, const int8_t* map, int32_t mapSizeX, int32_t mapSizeY)
{
	char c;
	for (int32_t y = 0; y < mapSizeY; ++y)
	{
		for (int32_t x = 0; x < mapSizeX; ++x)
		{
			if(map[y*mapSizeX + x] == 0)
			{
				c = ' ';
			} else {
				c = map[y*mapSizeX + x] + '0';
			}
			out << c;
		}
		out << std::endl;
	}
}

void writeMapStatistics(std::ostream& out, const uint8_t* map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel)
{
	
	int32_t groundCount = 0;
	int32_t underwaterCount = 0;
	int32_t maxH = -1;
	int32_t minH = 256;
	int32_t avAltitude = 0;
	int32_t avSeaDepth = 0;
	
	for (int32_t y = 0; y < mapSizeY; ++y)
	{
		for (int32_t x = 0; x < mapSizeX; ++x)
		{
			auto height = map[y*mapSizeX + x];
			if(height > waterLevel)
			{
				avAltitude += (height - waterLevel);
				++groundCount;
			} else {
				avSeaDepth += (waterLevel - height);
				++underwaterCount;
			}
			
			if(maxH < height)
			{
				maxH = height;
			}
			
			if(minH > height)
			{
				minH = height;
			}
			
		}
	}
	out << " *** Stats *** " << std::endl;
	out << "Ground: " << (groundCount*100)/(mapSizeX*mapSizeY) << "%" << std::endl;
	out << "Water: " << (underwaterCount*100)/(mapSizeX*mapSizeY) << "%" << std::endl;

	out << "Max mountain height: " << maxH - waterLevel << std::endl;
	out << "Average mountain height: " << avAltitude / groundCount << std::endl;
	
	out << "Max water depth: " << waterLevel - minH << std::endl;
	out << "Average water depth: " << avSeaDepth / underwaterCount << std::endl;
	
	out << "Max height: " << maxH << std::endl;
	out << "Min height: " << minH << std::endl;
	
}
