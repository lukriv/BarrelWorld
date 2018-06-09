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
	float temp = cell.GetContent(level)->m_temperature - 273;
	if(temp < min)
	{
		min = temp;
	}
				
	if(temp > max)
	{
		max = temp;
	}
}

void GetMaxMinTemperature(float temperature, float& max, float& min)
{
	float temp = temperature - 273;
	if(temp < min)
	{
		min = temp;
	}
				
	if(temp > max)
	{
		max = temp;
	}
}

void GetMaxMin(float temp, float& max, float& min)
{
	if(temp < min)
	{
		min = temp;
	}
				
	if(temp > max)
	{
		max = temp;
	}
}

void writeClimateStatistics(std::ostream& out, ClimateMap &map, int32_t waterLevel)
{
	static float lastTotalAirMass = 0;
	static float lastTotalWaterMass = 0;
	static float lastTotalIceMass = 0;
	static float lastTotalH2OMass = 0;
	
	float totalAirMass = 0;
	float totalWaterMass = 0;
	float totalIceMass = 0;
	
	float groundTempMax = -51;
	float groundTempMin = 151;
	int32_t groundCount = 0;
	float groundAvg = 0.0;
	
	float lowWaterTempMax = -51;
	float lowWaterTempMin = 151;
	int32_t lowWaterCount = 0;
	float lowWaterAvg = 0.0;
	
	//float deepWaterTempMax = -51;
	//float deepWaterTempMin = 151;
	
	float airTempMax = -51;
	float airTempMin = 151;
	int32_t airCount = 0;
	float airAvg = 0.0;

	//float waterStreamMax = 0;
	//float waterStreamMin = 5000;
	//float waterStreamAvg = 0.0;

	float airWindMax = 0;
	float airWindMin = 5000;
	float airWindAvg = 0.0;
	
	float airWindHighMax = 0;
	float airWindHighMin = 5000;
	float airWindHighAvg = 0.0;
	
	float windSpeed;
	
	for (int32_t y = 0; y < map.GetSizeY(); ++y)
	{
		for (int32_t x = 0; x < map.GetSizeX(); ++x)
		{
			ClimateCell& cell = map.GetCellValue(x,y);
			
			if(cell.IsCheckContent(2, CellContent::WATER))
			{
				//GetMaxMinTemperature(cell, 2, deepWaterTempMax, deepWaterTempMin);
			}
			
			if(cell.IsCheckContent(1, CellContent::WATER))
			{
				GetMaxMinTemperature(cell, 1, lowWaterTempMax, lowWaterTempMin);
				lowWaterAvg += cell.GetContent(1)->m_temperature - 273;
				
				totalWaterMass += cell.GetWaterContent().m_waterMass;
				totalIceMass += cell.GetWaterContent().m_iceMass;
				
				//windSpeed = cell.GetWaterContent().m_streamDir.Length();
				//GetMaxMin(windSpeed, waterStreamMax, waterStreamMin);
				//waterStreamAvg += windSpeed;
				
				++lowWaterCount;
			}
		
			if(cell.IsCheckContent(1, CellContent::GROUND))
			{
				GetMaxMinTemperature(cell, 1, groundTempMax, groundTempMin);	
				groundAvg += cell.GetContent(1)->m_temperature - 273;
				
				totalWaterMass += cell.GetGroundContent().m_waterMass;
				
				++groundCount;
			}
			
			if(cell.IsCheckContent(0, CellContent::AIR))
			{
				GetMaxMinTemperature(cell, 0, airTempMax, airTempMin);
				airAvg += cell.GetContent(0)->m_temperature - 273;
				
				windSpeed = cell.GetAirContent().m_lowWind.Length();
				GetMaxMin(windSpeed, airWindMax, airWindMin);
				airWindAvg += windSpeed;
				
				windSpeed = cell.GetAirContent().m_highWind.Length();
				GetMaxMin(windSpeed, airWindHighMax, airWindHighMin);
				airWindHighAvg += windSpeed;
				
				totalWaterMass += cell.GetAirContent().m_waterMass;
				totalAirMass += cell.GetAirContent().m_airMass;
				
				++airCount;
			}
			
			
		}
	}
	out << " *** Climate Stats *** " << std::endl;
	
	out << "Ground temperature max: " << groundTempMax << " C" << std::endl;
	out << "Ground temperature min: " << groundTempMin << " C" << std::endl;
	out << "Ground temperature average: " << groundAvg / (float)groundCount << " C" << std::endl;

	//out << "Deep water temperature max: " << deepWaterTempMax << " C" << std::endl;
	//out << "Deep water temperature min: " << deepWaterTempMin << " C" << std::endl;	
	
	out << "Low water temperature max: " << lowWaterTempMax << " C" << std::endl;
	out << "Low water temperature min: " << lowWaterTempMin << " C" << std::endl;	
	//out << "Low water deep temperature max: " << lowWaterDeepTempMax << " C" << std::endl;
	//out << "Low water deep temperature min: " << lowWaterDeepTempMin << " C" << std::endl;
	out << "Low water temperature average: " << lowWaterAvg / (float)lowWaterCount << " C" << std::endl;	
	
	//out << "Low water stream max: " << waterStreamMax << " m/s" << std::endl;
	//out << "Low water stream min: " << waterStreamMin << " m/s" << std::endl;	
	//out << "Low water stream average: " << waterStreamAvg / (float)lowWaterCount << " m/s" << std::endl;	
	
	out << "Air temperature max: " << airTempMax << " C" << std::endl;
	out << "Air temperature min: " << airTempMin << " C" << std::endl;	
	out << "Air temperature average: " << airAvg / (float)airCount << " C" << std::endl;
	
	out << "Air wind high max: " << airWindHighMax << " m/s" << std::endl;
	out << "Air wind high min: " << airWindHighMin << " m/s" << std::endl;	
	out << "Air wind high average: " << airWindHighAvg / (float)airCount << " m/s" << std::endl;
	
	out << "Air wind max: " << airWindMax << " m/s" << std::endl;
	out << "Air wind min: " << airWindMin << " m/s" << std::endl;	
	out << "Air wind average: " << airWindAvg / (float)airCount << " m/s" << std::endl;
	
	out << " *** Climate Check *** " << std::endl; 
	out << "Total liquid water mass difference: " << totalWaterMass - lastTotalWaterMass << " kg" << std::endl;
	out << "Total ice mass difference: " << totalIceMass - lastTotalIceMass << " kg" << std::endl;
	out << "Total H2O mass difference: " << totalWaterMass + totalIceMass - lastTotalH2OMass << " kg" << std::endl;
	out << "Total air mass difference: " << totalAirMass - lastTotalAirMass << " kg" << std::endl;
	
	if(lastTotalAirMass == 0)
	{
		lastTotalWaterMass = totalWaterMass;
		lastTotalIceMass = totalIceMass;
		lastTotalAirMass = totalAirMass;
		lastTotalH2OMass = totalWaterMass + totalIceMass;
	}

	
}

void writeCellStats(std::ostream& out, ClimateMap& map, int32_t x, int32_t y, int32_t level)
{
	ClimateCell &cell = map.GetCellValue(x, y);
	CellContent *pCont = map.GetCellValue(x, y).GetContent(level);
	if(pCont)
	{
		switch(pCont->GetContentType())
		{
			case CellContent::AIR:
				writeAirStats(out, cell, x, y);
				break;
			case CellContent::WATER:
				writeWaterStats(out, cell, x, y);
				break;
			case CellContent::GROUND:
				writeGroundStats(out, cell, x, y);
				break;
			default:
				out << " *** Not implemented yet ***" << std::endl;
				break;
		}
	} else {
		out << " *** ALL CONTENT ***" << std::endl;
		writeAirStats(out, cell, x, y);
		if(cell.IsCheckContent(CellContent::WATER))
		{
			writeWaterStats(out, cell, x, y);
		} else {
			writeGroundStats(out, cell, x, y);
		}
	}
}

void writeAirStats(std::ostream& out, ClimateCell& cell, int32_t x, int32_t y)
{
	AirContent *pAir = &cell.GetAirContent();
	out << "*** AIR CONTENT [ " << x << ", " << y <<"] ***" << std::endl;
	out << " Temperature: " << pAir->m_temperature - 273 << "[C]" << std::endl;
	out << " Air mass: " << pAir->m_airMass << "[kg]" << std::endl;
	out << " Altitude: " << pAir->m_baseAltitude << "[m]"<< std::endl;
	out << " Water mass: " << pAir->m_waterMass << "[kg]" << std::endl;
	out << " Pressure: " << pAir->m_airPressure << "[Pa]" << std::endl;
	out << " Dynamic pressure (low): " << pAir->m_dynamicPressureLow << "[Pa]" << std::endl;
	out << " Dynamic pressure (high): " << pAir->m_dynamicPressureHigh << "[Pa]" << std::endl;
	out << " Humidity: " << pAir->m_actHum << "[kg/kg]" << std::endl;
	out << " Clouds height: " << pAir->m_cloudsHeight << "[m]" << std::endl;
	out << " Precipitation: " << (int)pAir->m_precip << std::endl;
	out << " Volume level: " << pAir->m_volumeLevel << " [m] or [m^3]" << std::endl;
	//out << " Pressure force pulse high: " << pAir->m_highForcePulse.x_ << "," << pAir->m_highForcePulse.y_ << "; size: " << pAir->m_highForcePulse.Length() << std::endl;
	//out << " Pressure force high: " << pAir->m_highForce.x_ << "," << pAir->m_highForce.y_ << "; size: " << pAir->m_highForce.Length() << std::endl;
	out << " Dynamic force high: " << pAir->m_dynamicForceHigh.x_ << "," << pAir->m_dynamicForceHigh.y_ << "; size: " << pAir->m_dynamicForceHigh.Length() << std::endl;
	out << " High wind: " << pAir->m_highWind.x_ << "," << pAir->m_highWind.y_ << "; speed: " << pAir->m_highWind.Length() << std::endl;
	//out << " Pressure force pulse low: " << pAir->m_lowForcePulse.x_ << "," << pAir->m_lowForcePulse.y_ << "; size: " << pAir->m_lowForcePulse.Length() << std::endl;
	out << " Dynamic force: " << pAir->m_dynamicForce.x_ << "," << pAir->m_dynamicForce.y_ << "; size: " << pAir->m_dynamicForce.Length() << std::endl;
	//out << " Pressure force low: " << pAir->m_lowForce.x_ << "," << pAir->m_lowForce.y_ << "; size: " << pAir->m_lowForce.Length() << std::endl;
	out << " Low wind: " << pAir->m_lowWind.x_ << "," << pAir->m_lowWind.y_ << "; speed: " << pAir->m_lowWind.Length() << std::endl;
	
}

void writeGroundStats(std::ostream& out, ClimateCell& cell, int32_t x, int32_t y)
{
	GroundContent *pGround = &cell.GetGroundContent();
	out << "*** GROUND CONTENT [ " << x << ", " << y <<"] ***" << std::endl;
	out << " Temperature: " << pGround->m_temperature - 273 << " C" << std::endl;
	out << " Water mass: " << pGround->m_waterMass << " kg" << std::endl;
	out << " Water mass flow: " << pGround->m_waterFlow << " kg" << std::endl;
	out << " Water flow cycles: " << pGround->m_waterFlowCont << std::endl;
}

void writeWaterStats(std::ostream& out, ClimateCell& cell, int32_t x, int32_t y)
{
	WaterContent *pWater = &cell.GetWaterContent();
	out << "*** WATER CONTENT [ " << x << ", " << y <<"] ***" << std::endl;
	out << " Temperature: " << pWater->m_temperature - 273 << " C" << std::endl;
	out << " Water mass: " << pWater->m_waterMass << " kg" << std::endl;
	out << " Salt mass: " << pWater->m_saltMass << " kg" << std::endl;
	out << " Ice mass: " << pWater->m_iceMass << " kg" << std::endl;
	out << " Evaporated mass: " << pWater->m_evapWaterMass << " kg" << std::endl;
	//out << " Surface level:" << pWater->m_surfaceLevel << std::endl;
	//out << " Stream: " << pWater->m_streamDir.x_ << "," << pWater->m_streamDir.y_ << "; speed: " << pWater->m_streamDir.Length() << std::endl;
}
