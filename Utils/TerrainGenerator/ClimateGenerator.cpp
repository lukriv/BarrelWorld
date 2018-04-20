#include "ClimateGenerator.h"

#include <iostream>

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
const float SUN_POWER = 1000; // power [W] per square meter

const float HEAT_OF_VAPORIZATION_OF_WATER = 2300000; // [ J/kg ]

const float ENTHALPY_OF_FUSION_OF_WATER = 335000; // [ J/kg ]

const float WATER_FREEZING = 273; // freeze temperature [ K ]

const float RAIN_HEIGHT = 1000; // 


float CompAirMass(AirContent& cont, float baseAlt)
{
	// (1 / 2) * (1 / 11000) == 0.00004545
	//float height = 11000 - baseAlt;
	//float relPressure = ClimateUtils::GetPressureInHeight(baseAlt, cont.m_airPressure) / AIR_BASE_PRESSURE;
	//return (ClimateUtils::GetAirDensity(cont.m_temperature) * relPressure * 0.00004545 * height * height );
	
	// F/S = p, S==1m^2, F=m*g => m=p/g; g==10
	return ClimateUtils::GetPressureInHeight(baseAlt, cont.m_airPressure) / 10;
}

float CompAirMass(AirContent& cont)
{
	return CompAirMass(cont, cont.m_baseAltitude);
}



float ClimateGenerator::CompCloudsHeight(AirContent& cont)
{
	// (1 / 2) * (1 / 11000) == 0.00004545
	float dewPointTemp = ClimateUtils::CompDewPointTemperature(cont.m_actHum, cont.m_airPressure);
	
	return ClimateUtils::CompAltFromTemp(cont.m_temperature,cont.m_baseAltitude,dewPointTemp);
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
	return  cont.m_iceMass + cont.m_waterMass + cont.m_saltMass;
}

void AddHeat(WaterContent &cont, float heat)
{
	if(std::isnan(heat))
	{
		std::cout << "Error: Heat is nan" << std::endl;
		return;
	}
	
	if(heat > 0)
	{
		if(cont.m_iceMass == 0.0)
		{
			cont.m_temperature += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER));
		} else {
			if(cont.m_temperature >= WATER_FREEZING)
			{
				float iceMassDiff = -(heat / ENTHALPY_OF_FUSION_OF_WATER);
				if(cont.m_iceMass > iceMassDiff)
				{
					cont.m_waterMass -= iceMassDiff;
					cont.m_iceMass += iceMassDiff;
				} else {
					heat = heat - (cont.m_iceMass*ENTHALPY_OF_FUSION_OF_WATER);
					cont.m_waterMass += cont.m_iceMass; // add water
					cont.m_iceMass = 0.0; // no ice 
					cont.m_temperature += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER)); // add rest of heat to temperature
				}
			} else {
				cont.m_temperature += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::ICE));
			}
		}
	} else {
		if(cont.m_waterMass > 0.0)
		{
			if(cont.m_temperature > WATER_FREEZING)
			{
				cont.m_temperature += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER));
			} else {
				float iceMassDiff = -(heat / ENTHALPY_OF_FUSION_OF_WATER);
				if(cont.m_waterMass > iceMassDiff)
				{
					cont.m_waterMass -= iceMassDiff;
					cont.m_iceMass += iceMassDiff;
				} else {
					heat = heat + (cont.m_waterMass*ENTHALPY_OF_FUSION_OF_WATER);
					cont.m_iceMass += cont.m_waterMass;
					cont.m_waterMass = 0.0;
					cont.m_temperature += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::ICE)); // 
				}
			}
			
		} else {
			cont.m_temperature += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::ICE));
		}
	}
	
}

void AddHeat(AirContent &cont, float heat)
{
	float temp = heat / (cont.m_airMass * ClimateUtils::GetSpecificHeat(CellContent::AIR));
	cont.m_temperature += temp;
	cont.m_temperatureDiff += temp;
}

void AddHeat(GroundContent &cont, float heat)
{
	cont.m_temperature += heat / (CompGroundMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::GROUND));
}

void UpdateAirContent(AirContent &cont)
{
	cont.m_airPressure += cont.m_airPressureDiff;
	cont.m_airPressureDiff = 0.0;
	
	cont.m_airMass = CompAirMass(cont);
	cont.m_actHum = cont.m_waterMass/cont.m_airMass;
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
			
			if(pAir)
			{
				UpdateAirContent(*pAir);
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
	
	AirMoveStep();
		
	//AddHeatIncToClimate();
	
	//add sun step by one hour
	m_sunPosX = (m_sunPosX + m_sunStep) % m_climateMap.GetSizeX();
}


void ClimateGenerator::SunHeatingStep()
{
	
	float degreeStep = PI / m_map.GetSizeY();
	
	float degreeY = - (PI / 2);
	float cosY;
	
	float totalEnergy = 0;
	float totalEnergyGround = 0;
	
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
				AddHeat(air, (totalEnergy * ATHMOSPHERIC_ABSORBTION));
				
				// is clouds
				if(CompCloudsHeight(air) < 10000)
				{
					totalEnergyGround -= (totalEnergy * CLOUDS_SCATTERED);
				}
			}
			
			// air absortion
			totalEnergyGround -= (totalEnergy * ATHMOSPHERIC_ABSORBTION);
			
			level = 1;
			if(cell.IsCheckContent(level, CellContent::GROUND))
			{
				AddHeat(*cell.GetGroundContent(level), totalEnergyGround);
			}
			
			if(cell.IsCheckContent(level, CellContent::WATER))
			{
				AddHeat(*cell.GetWaterContent(level), totalEnergyGround);
			}
			degreeX += degreeStep;
		}
		degreeY += degreeStep;
	}
}

void ClimateGenerator::CoolingStep()
{
	float energyLoss = 0.0;
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
				
				AddHeat(*pAir, energyLoss);
			}
			
			if(cell.IsCheckContent(1, CellContent::GROUND))
			{
				pGround = cell.GetGroundContent(1);
				
				energyLoss = ClimateUtils::GetThermalConductivity(CellContent::GROUND) * ( pAir->m_temperature - pGround->m_temperature ) * 3600;

				AddHeat(*pAir, -energyLoss);
				AddHeat(*pGround, energyLoss);
				
			}
			
			if(cell.IsCheckContent(1, CellContent::WATER))
			{
				pLowWater = cell.GetWaterContent(1);
				
				energyLoss = ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( pAir->m_temperature - pLowWater->m_temperature ) * 3600;

				AddHeat(*pAir, -energyLoss);
				AddHeat(*pLowWater, energyLoss);
				
			}
			
			if(cell.IsCheckContent(2, CellContent::WATER))
			{
				pDeepWater = cell.GetWaterContent(2);
				
				energyLoss = ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( pLowWater->m_temperature - pDeepWater->m_temperature ) * 3600;

				AddHeat(*pLowWater, -energyLoss);
				AddHeat(*pDeepWater, energyLoss);
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
			AirContent &air = *cell.GetAirContent(0);
			
			AirPressureChange(air);
			
			if(cell.IsCheckContent(1, CellContent::WATER))
			{
				Evaporation(air, *cell.GetWaterContent(1));
			}
			
			UpdateAirContent(air);
			
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
	
	if(water.m_temperature < WATER_FREEZING)
	{
		return;
	}
	
	float maxHum = ClimateUtils::CompMaxAirHumidity((float)air.m_temperature, air.m_airPressure);
	
	float lenght = air.m_lowDir.Length();
	float waterMassEvap = (25.0 + 19.0*lenght)*(maxHum - air.m_actHum);
	float heat = HEAT_OF_VAPORIZATION_OF_WATER * waterMassEvap;
	
	water.m_waterMass -= waterMassEvap;
	air.m_waterMass += waterMassEvap;
	
	AddHeat(water, -heat);
	
}

void ClimateGenerator::AirMoveStep()
{
	for(int32_t y = 0; y < m_map.GetSizeY(); ++y)
	{
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x)
		{
			
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			AirContent &air = *cell.GetAirContent(0);
			
			AirMoveChange(air, x,y);

			
		}
	}
}

void ClimateGenerator::AirMoveChange(AirContent& air, int32_t x, int32_t y)
{
	int32_t nx, ny;
	Urho3D::Vector2 topDiff;
	Urho3D::Vector2 lowDiff;
	int32_t topCount = 0;
	int32_t lowCount = 0;
				
	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
	{
		
		m_climateMap.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), x, y, nx, ny);
		
		AirContent &nAir = *m_climateMap.GetCellValue(nx,ny).GetAirContent(0);
		
		
		if(air.m_airPressure > nAir.m_airPressure)
		{
			float dp = (air.m_airPressure - nAir.m_airPressure) / 2;
			float dv = dp*std::sqrt(2/(ClimateUtils::GetAirDensity(air.m_temperature)*(air.m_airPressure - dp)));
			
			Urho3D::Vector2 vec;
			switch(static_cast<MapContainerDIR>(dir))
			{
			case MapContainerDIR::N:
				vec.x_ = 0; vec.y_ = -1; break;
			case MapContainerDIR::NW:
				vec.x_ = -0.7071; vec.y_ = -0.7071; break;
			case MapContainerDIR::W:
				vec.x_ = -1; vec.y_ = 0; break;
			case MapContainerDIR::SW:
				vec.x_ = -0.7071; vec.y_ = 0.7071; break;
			case MapContainerDIR::S:
				vec.x_ = 0; vec.y_ = 1; break;
			case MapContainerDIR::SE:
				vec.x_ = 0.7071; vec.y_ = 0.7071; break;
			case MapContainerDIR::E:
				vec.x_ = 1; vec.y_ = 0; break;
			case MapContainerDIR::NE:
				vec.x_ = 0.7071; vec.y_ = -0.7071; break;
			}
			
			vec = vec*dv;
			
			if(air.m_temperature >= nAir.m_temperature)
			{
				++topCount;
				topDiff += vec;
			} else {
				++lowCount;
				lowDiff += vec;
			}

		}
		
	}
	
	topDiff = (topCount > 0) ? topDiff / ((float)topCount) : Urho3D::Vector2::ZERO;
	lowDiff = (lowCount > 0) ? lowDiff / ((float)lowCount) : Urho3D::Vector2::ZERO;
	
	//if(std::isnan(topDiff.x_)
	//	||std::isnan(topDiff.y_)
	//	||std::isnan(lowDiff.x_)
	//	||std::isnan(lowDiff.y_))
	//{
	//	std::cout << "Error vole" << std::endl;
	//}
	
	air.m_highDir += topDiff;
	air.m_lowDir += lowDiff;
}

void ClimateGenerator::AirWind(AirContent& air, int32_t x, int32_t y)
{
	int32_t nx, ny;
	// get receiver
	float speed = 0.0f;
	speed = air.m_lowDir.Length();
	if(speed > 0.0)
	{
		m_climateMap.GetNeightbourCoords(GetWindDest(air.m_lowDir), nx, ny);
		AirContent &nAir = m_climateMap.GetCellValue(nx, ny);
		
	}
}

MapContainerDIR ClimateGenerator::GetWindDest(Urho3D::Vector2& windVec)
{
	float angle = windVec.Angle(Urho3D::Vector2(0.0, 1.0));
	
	if(angle > 0)
	{
		if(angle < 22.5)
		{
			return MapContainerDIR::N;
		} else if (angle < 67.5) {
			return MapContainerDIR::NE;
		} else if (angle < 112.5) {
			return MapContainerDIR::E;
		} else if (angle < 157.5) {
			return MapContainerDIR::SE;
		} else {
			return MapContainerDIR::S;		
		}
	} else {
		if(angle > -22.5)
		{
			return MapContainerDIR::N;
		} else if (angle > -67.5) {
			return MapContainerDIR::NW;
		} else if (angle > 112.5) {
			return MapContainerDIR::W;
		} else if (angle > 157.5) {
			return MapContainerDIR::SW;
		} else {
			return MapContainerDIR::S;
		}
	}
}




