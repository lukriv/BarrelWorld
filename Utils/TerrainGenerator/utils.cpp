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

void GetMaxMinTemperature(ClimateCell &cell, int32_t level, float& max, float& min)
{
	int32_t temp = cell.GetContent(level)->m_temperature - 273;
	if(temp < min)
	{
		min = temp;
	}
				
	if(temp > max)
	{
		max = temp;
	}
}

void writeClimateStatistics(std::ostream& out, MapContainer<ClimateCell, SphereMapCoords> &map, int32_t waterLevel)
{
	
	float groundTempMax = -51;
	float groundTempMin = 151;
	int32_t groundCount = 0;
	float groundAvg = 0.0;
	
	float lowWaterTempMax = -51;
	float lowWaterTempMin = 151;
	int32_t lowWaterCount = 0;
	float lowWaterAvg = 0.0;
	
	float deepWaterTempMax = -51;
	float deepWaterTempMin = 151;
	
	float airTempMax = -51;
	float airTempMin = 151;
	int32_t airCount = 0;
	float airAvg = 0.0;

	
	for (int32_t y = 0; y < map.GetSizeY(); ++y)
	{
		for (int32_t x = 0; x < map.GetSizeX(); ++x)
		{
			ClimateCell& cell = map.GetCellValue(x,y);
			
			if(cell.IsCheckContent(2, CellContent::WATER))
			{
				GetMaxMinTemperature(cell, 2, deepWaterTempMax, deepWaterTempMin);
			}
			
			if(cell.IsCheckContent(1, CellContent::WATER))
			{
				GetMaxMinTemperature(cell, 1, lowWaterTempMax, lowWaterTempMin);
				lowWaterAvg += cell.GetContent(1)->m_temperature - 273;
				++lowWaterCount;
			}
		
			if(cell.IsCheckContent(1, CellContent::GROUND))
			{
				GetMaxMinTemperature(cell, 1, groundTempMax, groundTempMin);	
				groundAvg += cell.GetContent(1)->m_temperature - 273;
				++groundCount;
			}
			
			if(cell.IsCheckContent(0, CellContent::AIR))
			{
				GetMaxMinTemperature(cell, 0, airTempMax, airTempMin);
				airAvg += cell.GetContent(0)->m_temperature - 273;
				++airCount;
			}
			
			
		}
	}
	out << " *** Climate Stats *** " << std::endl;
	out << "Ground temperature max: " << groundTempMax << " C" << std::endl;
	out << "Ground temperature min: " << groundTempMin << " C" << std::endl;
	out << "Ground temperature average: " << groundAvg / (float)groundCount << " C" << std::endl;

	out << "Deep water temperature max: " << deepWaterTempMax << " C" << std::endl;
	out << "Deep water temperature min: " << deepWaterTempMin << " C" << std::endl;	
	
	out << "Low water temperature max: " << lowWaterTempMax << " C" << std::endl;
	out << "Low water temperature min: " << lowWaterTempMin << " C" << std::endl;	
	out << "Low water temperature average: " << lowWaterAvg / (float)lowWaterCount << " C" << std::endl;	
	
	out << "Air temperature max: " << airTempMax << " C" << std::endl;
	out << "Air temperature min: " << airTempMin << " C" << std::endl;	
	out << "Air temperature average: " << airAvg / (float)airCount << " C" << std::endl;
	
}
