#include "ClimateGenerator.h"

#include <cmath>
#include "ClimateUtils.h"

#define PI 3.14159265

const float WATER_SPECIFIC = 334000; // [J] merne skupenske teplo tani vody

// kilometer
const uint8_t LOW_AIR_HEIGHT = 10;
const uint8_t LOW_WATER_DEEP = 1;
const float SQUARE_SIZE = 1000* 1000;

const float GROUND_WATER_METERS = 5;

const float SALT_MASS_ON_SQUARE_METER = 35; // kilo per cubic meter

const int32_t DAYLONG = 24; // hours

const float GROUND_HEIGHT_TO_METERS = 100;

const float AIR_BASE_PRESSURE = 100000.0;



const float DEEP_WATER_BASE_TEMPERATURE = 4 + 273;
const float BASE_TEMPERATURE = 15 + 273;


const float ATHMOSPHERIC_ABSORBTION =  	0.175; // 

const float CLOUDS_SCATTERED =  0.145; // 
const float SUN_POWER = 100; // power [W] per square meter

const float HEAT_OF_VAPORIZATION_OF_WATER = 2300000; // [ J/kg ]

const float ENTHALPY_OF_FUSION_OF_WATER = 335000; // [ J/kg ]


float CompAirMass(AirContent& cont)
{
	// (1 / 2) * (1 / 11000) == 0.00004545
	float height = 11000 - cont.m_baseAltitude;
	return (ClimateUtils::GetAirDensity(cont.m_temperature) * (cont.m_airPressure / AIR_BASE_PRESSURE) * 0.00004545 * height * height );
}

float CompGroundMass(GroundContent& cont)
{
	return ( GROUND_WATER_METERS * ClimateUtils::GetGroundDensity() + cont.m_waterMass);
}

float CompWaterSalinity(WaterContent &cont)
{
	return (cont.m_saltMass / cont.m_waterMass) * 1000;
}

float CompWaterMass(WaterContent& cont)
{
	
	return (cont.m_iceMass > 0) ? cont.m_iceMass : cont.m_waterMass + cont.m_saltMass;
}

float GetWaterSpecificHeat(WaterContent& cont)
{
	return (cont.m_iceMass > 0) ? ClimateUtils::GetSpecificHeat(CellContent::ICE) : ClimateUtils::GetSpecificHeat(CellContent::WATER);
}



ClimateGenerator::~ClimateGenerator()
{
}

void ClimateGenerator::InitializeClimate()
{
	m_climateMap.Initialize(m_map.GetSizeX(), m_map.GetSizeY());
	uint8_t height = 0;
	
	WaterContent* pWater = nullptr;
	AirContent* pAir = nullptr;
	
	for(int32_t y = 0; y < m_map.GetSizeY(); ++y)
	{
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x)
		{
			ClimateCell& cell = m_climateMap.GetCellValue(x, y);
			height = m_map.GetCellValue(x, y);
			if(height < (m_waterLevel - LOW_WATER_DEEP))
			{
				// deep water
				pWater = new WaterContent();
				pWater->m_saltMass = (float)(m_waterLevel - LOW_WATER_DEEP - height) *GROUND_HEIGHT_TO_METERS * SALT_MASS_ON_SQUARE_METER; // kg
				pWater->m_waterMass = (float)(m_waterLevel - LOW_WATER_DEEP - height) *GROUND_HEIGHT_TO_METERS * 1000; // kg of water
				pWater->m_temperature = DEEP_WATER_BASE_TEMPERATURE;
				cell.AddContent(2, pWater);
				// low water
				pWater = new WaterContent();
				pWater->m_saltMass = (float)(LOW_WATER_DEEP) *GROUND_HEIGHT_TO_METERS * SALT_MASS_ON_SQUARE_METER ; // kg
				pWater->m_waterMass = (float)(LOW_WATER_DEEP) *GROUND_HEIGHT_TO_METERS * 1000; // kg of water
				pWater->m_temperature = BASE_TEMPERATURE;
				cell.AddContent(1, pWater);
				// low air
				pAir = new AirContent();
				pAir->m_airPressure = AIR_BASE_PRESSURE;
				pAir->m_temperature = BASE_TEMPERATURE;
				cell.AddContent(0, pAir);
			} else if(height < m_waterLevel) {
				// low water
				pWater = new WaterContent();
				pWater->m_saltMass = (float)(m_waterLevel - height) *GROUND_HEIGHT_TO_METERS * SALT_MASS_ON_SQUARE_METER ; // kg
				pWater->m_waterMass = (float)(m_waterLevel - height) *GROUND_HEIGHT_TO_METERS * 1000; // kg of water
				pWater->m_temperature = BASE_TEMPERATURE;
				cell.AddContent(1, pWater);
				// low air
				pAir = new AirContent();
				pAir->m_airPressure = AIR_BASE_PRESSURE;
				pAir->m_temperature = BASE_TEMPERATURE;
				cell.AddContent(0, pAir);
				
			} else  {
				// low air
				pAir = new AirContent();
				pAir->m_baseAltitude = (height - m_waterLevel) * GROUND_HEIGHT_TO_METERS;
				pAir->m_airPressure = AIR_BASE_PRESSURE;
				pAir->m_temperature = ClimateUtils::CompRealAirTemp(BASE_TEMPERATURE, pAir->m_baseAltitude);
				cell.AddContent(0, pAir);

				// ground
				GroundContent* pGround = new GroundContent();
				pGround->m_maxWaterCapacity = SQUARE_SIZE * GROUND_WATER_METERS;
				pGround->m_temperature = pAir->m_temperature;
				cell.AddContent(1, pGround);
			}
			

		}
	}
	
}

ClimateGenerator::ClimateGenerator(MapContainer<uint8_t, SphereMapCoords>& map, MapContainer<ClimateCell, SphereMapCoords>& climateMap, int32_t waterLevel) : 
	m_waterLevel(waterLevel),m_map(map), m_climateMap(climateMap)
{
	InitializeClimate();
	m_sunPosX = 0;
	m_sunPosY = m_map.GetSizeY() / 2;
	m_sunRadius = m_map.GetSizeY() / 2;
	m_sunStep = map.GetSizeX() / DAYLONG;
	
	
}

void ClimateGenerator::SimulateClimateStep()
{
	SunHeatingStep();
	
	CoolingStep();
		
	AirPressureDiffStep();
		
	//AddHeatIncToClimate();
	
	//add sun step by one hour
	m_sunPosX = (m_sunPosX + m_sunStep) % m_climateMap.GetSizeX();
}


void ClimateGenerator::SunHeatingStep()
{
	
	float degreeStep = PI / m_map.GetSizeY();
	
	float degreeY = - (PI / 2);
	float cosY;
	
	float mass = 0;
	float totalEnergy = 0;
	float totalEnergyGround = 0;
	float temp = 0;
	
	int32_t level = 0;
	
	// add warm to the surface
	for(int32_t y = m_sunPosY - (m_map.GetSizeY() / 2); y < m_sunPosY + (m_map.GetSizeY() / 2); ++y)
	{
		float degreeX = - (PI / 2);
		cosY = std::abs(std::cos(degreeY));
		for(int32_t x = m_sunPosX - (m_map.GetSizeY() / 2); x < m_sunPosX + (m_map.GetSizeY() / 2); ++x)
		{
			ClimateCell& cell = m_climateMap.GetCellValue(x,y);
			totalEnergyGround = totalEnergy = SUN_POWER * cosY * std::abs(std::cos(degreeX)) * 3600; // energi per hour
			
			level = 0;
			if(cell.IsCheckContent(level, CellContent::AIR))
			{
				AirContent &air = *cell.GetAirContent(level);
				mass = CompAirMass(air);
				temp = (totalEnergy * ATHMOSPHERIC_ABSORBTION) / (mass * ClimateUtils::GetSpecificHeat(CellContent::AIR));
				
				air.m_temperature += temp;
				
				// set diff
				air.m_temperatureDiff = temp;
				
				// is clouds
				if((air.m_waterMass / mass) > ClimateUtils::GetMaxAirHumidity(air.m_temperature))
				{
					totalEnergyGround -= (totalEnergy * CLOUDS_SCATTERED);
				}
			}
			
			
			
			// air absortion
			totalEnergyGround -= (totalEnergy * ATHMOSPHERIC_ABSORBTION);
			
			level = 1;
			if(cell.IsCheckContent(level, CellContent::GROUND))
			{
				GroundContent &ground = *cell.GetGroundContent(level);
				mass = CompGroundMass(ground);
				temp = totalEnergyGround / (mass * ClimateUtils::GetSpecificHeat(CellContent::GROUND));
				ground.m_temperature += temp;
			}
			
			if(cell.IsCheckContent(level, CellContent::WATER))
			{
				WaterContent &water = *cell.GetWaterContent(level);
				mass = CompWaterMass(water);
				
				float specHeat = (water.m_iceMass > 0) ? ClimateUtils::GetSpecificHeat(CellContent::ICE) : ClimateUtils::GetSpecificHeat(CellContent::WATER);
				temp = totalEnergyGround / (mass * specHeat);
				water.m_temperature += temp;
			}
			degreeX += degreeStep;
		}
		degreeY += degreeStep;
	}
}

void ClimateGenerator::CoolingStep()
{
	float energyLoss = 0.0;
	float massAir = 0.0;
	float massLevel2 = 0.0;
	float massLevel3 = 0.0;
	float temp = 0.0;
	AirContent *pAir = nullptr;
	GroundContent *pGround = nullptr;
	WaterContent *pLowWater = nullptr;
	WaterContent *pDeepWater = nullptr;
	
	for(int32_t y = 0; y < m_map.GetSizeY(); ++y)
	{
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x)
		{
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			
			if(cell.IsCheckContent(0, CellContent::AIR))
			{
				pAir = cell.GetAirContent(0);
				
				energyLoss = ClimateUtils::GetThermalConductivity(CellContent::AIR) 
						* ( 173 - ClimateUtils::CompRealAirTemp(pAir->m_temperature, pAir->m_baseAltitude, 11000)) * 3600; // 
						
				massAir = CompAirMass(*pAir);
				
				temp = energyLoss / (massAir * ClimateUtils::GetSpecificHeat(CellContent::AIR));
				
				pAir->m_temperature += temp;
				pAir->m_temperatureDiff += temp;
			}
			
			if(cell.IsCheckContent(1, CellContent::GROUND))
			{
				pGround = cell.GetGroundContent(1);
				
				energyLoss = ClimateUtils::GetThermalConductivity(CellContent::GROUND) * ( pAir->m_temperature - pGround->m_temperature ) * 3600;

				massLevel2 = CompGroundMass(*pGround);
				
				temp = (-energyLoss) / (massAir * ClimateUtils::GetSpecificHeat(CellContent::AIR));
				pAir->m_temperature += temp;
				pAir->m_temperatureDiff += temp;
				
				pGround->m_temperature += energyLoss / (massLevel2 * ClimateUtils::GetSpecificHeat(CellContent::GROUND));
			}
			
			if(cell.IsCheckContent(1, CellContent::WATER))
			{
				pLowWater = cell.GetWaterContent(1);
				
				energyLoss = ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( pAir->m_temperature - pLowWater->m_temperature ) * 3600;

				float specHeat = (pLowWater->m_iceMass > 0) ? ClimateUtils::GetSpecificHeat(CellContent::ICE) : ClimateUtils::GetSpecificHeat(CellContent::WATER);
						
				massLevel2 = CompWaterMass(*pLowWater);

				temp = (-energyLoss) / (massAir * ClimateUtils::GetSpecificHeat(CellContent::AIR));
				pAir->m_temperature += temp;
				pAir->m_temperatureDiff += temp;
				pLowWater->m_temperature += energyLoss / (massLevel2 * specHeat);
			}
			
			if(cell.IsCheckContent(2, CellContent::WATER))
			{
				pDeepWater = cell.GetWaterContent(2);
				
				energyLoss = ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( pLowWater->m_temperature - pDeepWater->m_temperature ) * 3600;

				float specHeat = (pLowWater->m_iceMass > 0) ? ClimateUtils::GetSpecificHeat(CellContent::ICE) : ClimateUtils::GetSpecificHeat(CellContent::WATER);
						
				massLevel3 = CompWaterMass(*pDeepWater);
						
				pLowWater->m_temperature += (-energyLoss) / (massLevel2 * specHeat);
				
				specHeat = (pDeepWater->m_iceMass > 0) ? ClimateUtils::GetSpecificHeat(CellContent::ICE) : ClimateUtils::GetSpecificHeat(CellContent::WATER);
				
				pDeepWater->m_temperature += energyLoss / (massLevel3 * specHeat);
			}
		}
	}
}

void ClimateGenerator::AirPressureDiffStep()
{

	for(int32_t y = 0; y < m_map.GetSizeY(); ++y)
	{
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x)
		{
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			
			AirPressureChange(*cell.GetAirContent(0));
			
			if(cell.IsCheckContent(1, CellContent::WATER))
			{
				Evaporation(*cell.GetAirContent(0), *cell.GetWaterContent(1));
			}
	
			//massAir0 = CompHighAirMass(*pAir0);
			//massAir1 = CompLowAirMass(*pAir1);
			

			
		}
	}
}


void ClimateGenerator::AirPressureChange(AirContent& air)
{
	if (air.m_temperatureDiff != 0)
	{
		// compute new pressure
		air.m_airPressure = air.m_airPressure * (air.m_temperature/ (air.m_temperature - air.m_temperatureDiff));
		air.m_temperatureDiff = 0.0; // delete temperature diff
	}
}

void ClimateGenerator::Evaporation(AirContent& air, WaterContent& water)
{
	// bad - too much computing...:) 
	float airMass = CompAirMass(air);
	float maxHum = ClimateUtils::CompMaxAirHumidity((float)air.m_temperature, air.m_airPressure);
	
	float actHum = air.m_waterMass/airMass;
	float waterMassEvap = (25.0 + 19.0*air.m_lowDir.Length())*(maxHum - actHum);
	float heat = HEAT_OF_VAPORIZATION_OF_WATER * waterMassEvap;
	
	float dewPointTemp = ClimateUtils::CompDewPointTemperature(actHum, air.m_airPressure);
	
	float altitude = ClimateUtils::CompAltFromTemp(air.m_temperature,air.m_baseAltitude,dewPointTemp);
	
	water.m_waterMass -= waterMassEvap;
	air.m_waterMass += waterMassEvap;
	
	
	water.m_temperature -= heat/(CompWaterMass(water)*GetWaterSpecificHeat(water));
	
}





