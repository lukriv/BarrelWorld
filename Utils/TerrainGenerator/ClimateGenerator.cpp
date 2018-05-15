#include "ClimateGenerator.h"

#include <iostream>
#include <sstream>

#include <cmath>
#include "ClimateUtils.h"

#define PI 3.14159265

const float WATER_SPECIFIC = 334000; // [J] merne skupenske teplo tani vody

// kilometer
const uint8_t LOW_WATER_DEEP = 1;
const uint8_t LOW_WATER_METERS = 10; // deep of low water
const float SQUARE_SIDE_SIZE = 1000;
const float SQUARE_SIZE = SQUARE_SIDE_SIZE* SQUARE_SIDE_SIZE;

const float MEASURE_HEIGHT = 3000;



const float SALT_MASS_ON_SQUARE_METER = 35; // kilo per cubic meter

const int32_t DAYLONG = 24; // hours

const float TIME_STEP = 3600; // time step in seconds - hour

const float ANGULAR_VELOCITY = (2*PI)/(DAYLONG*TIME_STEP); // planet angular velocity

const float GROUND_HEIGHT_TO_METERS = 100;

const float AIR_BASE_PRESSURE = 101500.0;

//const float WATER_VISCOSITY = 0.000001; // [m^2/s]
//const float AIR_VISCOSITY = 0.0000133; // [m^2/s]

const float DEEP_WATER_BASE_TEMPERATURE = 4 + 273;
const float BASE_TEMPERATURE = 15 + 273;


const float MASS_MOVE_PROTECTION = 0.05; // maximal mass move ( ratio to whole mass )
const float WIND_SPEED_PROTECTION = 50; // maximal posible wind speed [m/s]
const float FORCE_SIZE_PROTECTION = 10000; // maximal posible force size [N]

const float MAX_AIR_PRESSURE_PROTECTION = 108000; // maximal air pressure [Pa]
const float MIN_AIR_PRESSURE_PROTECTION = 87000; // minimal air pressure [Pa]


const float HEAT_OF_VAPORIZATION_OF_WATER = 2300000; // [ J/kg ]

const float ENTHALPY_OF_FUSION_OF_WATER = 335000; // [ J/kg ]

const float WATER_FREEZING = 273; // freeze temperature [ K ]

const float RAIN_HEIGHT = 1000; // 

const float CLOADS_UNDER_HEIGHT = 5500;

// parameters
const float GROUND_WATER_METERS = 5;
const float GROUND_HEAT_METERS = 1;

const float ATHMOSPHERIC_ABSORBTION =  	0.175; // 

const float CLOUDS_SCATTERED =  0.145; // 
const float SUN_POWER = 1400; // power [W] per square meter

const float GRAV_ACC = 10;


float CompRealAirPressure(AirContent& cont)
{
	return cont.m_airMass * GRAV_ACC;
}

float CompDensity(AirContent& cont)
{
	return ClimateUtils::AIR_DENSITY*(CompRealAirPressure(cont)/AIR_BASE_PRESSURE);
	//return ClimateUtils::GetAirDensity(cont.m_temperature)*(CompRealAirPressure(cont)/AIR_BASE_PRESSURE);
}

float CompDensityInAltitude(AirContent& cont, float altitude)
{
	return ClimateUtils::AIR_DENSITY*(ClimateUtils::CompAbsolutePressure(altitude, cont.m_airPressure)/AIR_BASE_PRESSURE);
	//return ClimateUtils::GetAirDensity(cont.m_temperature)*(ClimateUtils::CompAbsolutePressure(altitude, cont.m_airPressure)/AIR_BASE_PRESSURE);
}

float CompAirMass(AirContent& cont, float baseAlt)
{
	return cont.m_airMass*ClimateUtils::CompAbsolutePressure(baseAlt, cont.m_airPressure)/cont.m_airPressure;
}

float CompAirPressure(AirContent& cont)
{
	if(cont.m_baseAltitude == 0)
	{
		return CompRealAirPressure(cont);
	} else {
		return ClimateUtils::CompRelativePressure(cont.m_baseAltitude, CompRealAirPressure(cont));
	}
}

float ClimateGenerator::CompCloudsHeight(AirContent& cont)
{
	// (1 / 2) * (1 / 11000) == 0.00004545
	float dewPointTemp = ClimateUtils::CompDewPointTemperature(cont.m_actHum, cont.m_airPressure);
	
	//if(std::isnan(dewPointTemp))
	//{
	//	std::cout << "Fuck my dick" << std::endl;
	//}
	
	return ClimateUtils::CompAltFromTemp(cont.m_temperature,cont.m_baseAltitude,dewPointTemp);
}

float CompGroundMass(GroundContent& cont)
{
	return ( GROUND_HEAT_METERS * ClimateUtils::GROUND_DENSITY + cont.m_waterMass);
}

float CompWaterSalinity(WaterContent &cont)
{
	return (cont.m_saltMass / cont.m_waterMass) * ClimateUtils::WATER_DENSITY;
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
			cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER));
		} else {
			if(cont.m_temperature >= WATER_FREEZING)
			{
				float iceMassDiff = heat / ENTHALPY_OF_FUSION_OF_WATER;
				if(cont.m_iceMass > iceMassDiff)
				{
					cont.m_waterMass += iceMassDiff;
					cont.m_iceMass -= iceMassDiff;
				} else {
					heat = heat - (cont.m_iceMass*ENTHALPY_OF_FUSION_OF_WATER);
					cont.m_waterMass += cont.m_iceMass; // add water
					cont.m_iceMass = 0.0; // no ice 
					cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER)); // add rest of heat to temperature
				}
			} else {
				cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::ICE));
			}
		}
	} else {
		if(cont.m_waterMass > 0.0)
		{
			if(cont.m_temperature > WATER_FREEZING)
			{
				cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER));
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
					cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::ICE)); // 
				}
			}
			
		} else {
			cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::ICE));
		}
	}
	
}

void AddHeat(AirContent &cont, float heat)
{
	float temp = heat / (cont.m_airMass * ClimateUtils::GetSpecificHeat(CellContent::AIR));
	//cont.m_temperature += temp;
	if((temp > 10)||(std::isnan(temp))||(std::isinf(temp)||(temp < -10)))
	{
		std::cout << "Warning! Suspicious temperature change" << std::endl;
	}
	
	cont.m_temperatureDiff += temp;
}

void AddHeat(GroundContent &cont, float heat)
{
	cont.m_temperatureDiff += heat / (CompGroundMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::GROUND));
}

bool UpdateContent(AirContent &cont)
{
	bool retval = true;
	
	// automatic volume level actualization
	//							// temperature change									//mass change
	cont.m_airMass += cont.m_airMassDiff;
	
	cont.m_temperature += cont.m_temperatureDiff;
	
	cont.m_waterMass += cont.m_waterMassDiff;
	
	cont.m_airPressure = CompAirPressure(cont);
	
	cont.m_volumeLevel = (cont.m_airMass - ClimateUtils::CompAbsolutePressure(cont.m_baseAltitude + MEASURE_HEIGHT, cont.m_airPressure)/GRAV_ACC)/ClimateUtils::GetAirDensity(cont.m_temperature)*(cont.m_airPressure/AIR_BASE_PRESSURE)
						- cont.m_volumeBase;
	
	// air mass, temperature and air pressure must be updated already
	float windSpeed = cont.m_lowWind.Length();
	cont.m_dynamicPressureLow = 0.5*windSpeed*windSpeed*ClimateUtils::AIR_DENSITY*(cont.m_airPressure/AIR_BASE_PRESSURE);
	
	//windSpeed = cont.m_highWind.Length();
	//cont.m_dynamicPressureHigh = 0.5*windSpeed*windSpeed*CompDensityInAltitude(cont, cont.m_baseAltitude + MEASURE_HEIGHT);

	
	cont.m_actHum = cont.m_waterMass/cont.m_airMass;
	cont.m_actHum = (cont.m_actHum > 0) ? cont.m_actHum : 0; // water mass can be lower than zero
	
	cont.m_cloudsHeight = ClimateGenerator::CompCloudsHeight(cont);
	cont.m_cloudsHeight = (cont.m_cloudsHeight < 0) ? 0 : cont.m_cloudsHeight; 
	
	if((cont.m_airMass < 0)
		||(cont.m_temperature < 0))
	{
		std::stringstream sstr;
		sstr << "Error! Air: cont.m_airMass: " << cont.m_airMass
					<< " cont.m_temperature: " << cont.m_temperature << std::endl;
					
		throw(new ClimateGenerator::ClimateException(sstr.str()));
	}
	
	cont.m_airMassDiff = 0.0;
	cont.m_temperatureDiff = 0.0;
	cont.m_waterMassDiff = 0.0;
	
	if(cont.m_waterMass < 0)
	{
		std::cout << "Warning! Air: cont.m_waterMass: " << cont.m_waterMass << std::endl;
		retval = false;
	}
	
	return retval;
}

bool UpdateContent(WaterContent &cont)
{
	bool retval = true;
	
	cont.m_temperature += cont.m_temperatureDiff;
	
	cont.m_waterMass += cont.m_waterMassDiff;
	
	cont.m_iceMass += cont.m_iceMassDiff;
	
	cont.m_surfaceLevel = (cont.m_waterMass/ClimateUtils::WATER_DENSITY) - LOW_WATER_METERS;
	
	float speed = cont.m_streamDir.Length();
	cont.m_dynamicPressure = 0.5*speed*speed*ClimateUtils::WATER_DENSITY;

	
	cont.m_temperatureDiff = 0.0;
	cont.m_waterMassDiff = 0.0;
	cont.m_iceMassDiff = 0.0;
	
	if((cont.m_waterMass < 0)
		||(cont.m_iceMass < 0)
		||(cont.m_temperature < 0))
	{
		std::stringstream sstr;
		sstr << "Error! Water: cont.m_waterMass: " << cont.m_waterMass
					<< " cont.m_iceMass: " << cont.m_iceMass
					<< " cont.m_temperature: " << std::endl;
		
		throw(new ClimateGenerator::ClimateException(sstr.str()));
	}
	
	return retval;
}

bool UpdateContent(GroundContent &cont)
{
	bool retval = true;
	cont.m_temperature += cont.m_temperatureDiff;
	cont.m_waterMass += cont.m_waterMassDiff;
	
	cont.m_temperatureDiff = 0.0;
	cont.m_waterMassDiff = 0.0;
	
	if(cont.m_waterMass < 0)
	{
		std::cout << "Warning! Ground cont.m_waterMass: " << cont.m_waterMass << std::endl;
		retval = false;
	}
	
	return retval;
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
				pWater->m_waterMass = (float)(m_waterLevel - LOW_WATER_DEEP - height) *GROUND_HEIGHT_TO_METERS * ClimateUtils::WATER_DENSITY; // kg of water
				pWater->m_temperature = DEEP_WATER_BASE_TEMPERATURE;
				cell.AddContent(2, pWater);
				// low water
				pWater = new WaterContent();
				pWater->m_saltMass = (float)(LOW_WATER_DEEP) *LOW_WATER_METERS * SALT_MASS_ON_SQUARE_METER ; // kg
				pWater->m_waterMass = (float)(LOW_WATER_DEEP) *LOW_WATER_METERS * ClimateUtils::WATER_DENSITY; // kg of water
				pWater->m_temperature = BASE_TEMPERATURE;
				cell.AddContent(1, pWater);
				// low air
				pAir = new AirContent();
				pAir->m_airMass = AIR_BASE_PRESSURE/GRAV_ACC;
				pAir->m_temperature = BASE_TEMPERATURE;
				pAir->m_volumeBase = (pAir->m_airMass - (ClimateUtils::CompAbsolutePressure(pAir->m_baseAltitude + MEASURE_HEIGHT, AIR_BASE_PRESSURE)/GRAV_ACC))/ClimateUtils::GetAirDensity(pAir->m_temperature);
				cell.AddContent(0, pAir);
			} else if(height < m_waterLevel) {
				// low water
				pWater = new WaterContent();
				pWater->m_saltMass = (float)(m_waterLevel - height) *LOW_WATER_METERS * SALT_MASS_ON_SQUARE_METER ; // kg
				pWater->m_waterMass = (float)(m_waterLevel - height) *LOW_WATER_METERS * ClimateUtils::WATER_DENSITY; // kg of water
				pWater->m_temperature = BASE_TEMPERATURE;
				cell.AddContent(1, pWater);
				// low air
				pAir = new AirContent();
				pAir->m_airMass = AIR_BASE_PRESSURE/GRAV_ACC;
				pAir->m_temperature = BASE_TEMPERATURE;
				pAir->m_volumeBase = (pAir->m_airMass - (ClimateUtils::CompAbsolutePressure(pAir->m_baseAltitude + MEASURE_HEIGHT, AIR_BASE_PRESSURE)/GRAV_ACC))/ClimateUtils::GetAirDensity(pAir->m_temperature);
				cell.AddContent(0, pAir);
				
			} else  {
				// low air
				pAir = new AirContent();
				pAir->m_baseAltitude = (height - m_waterLevel) * GROUND_HEIGHT_TO_METERS;
				pAir->m_airMass = ClimateUtils::CompAbsolutePressure(pAir->m_baseAltitude, AIR_BASE_PRESSURE)/GRAV_ACC;
				pAir->m_temperature = ClimateUtils::CompRealAirTemp(BASE_TEMPERATURE, pAir->m_baseAltitude);
				pAir->m_volumeBase = (pAir->m_airMass - (ClimateUtils::CompAbsolutePressure(pAir->m_baseAltitude + MEASURE_HEIGHT, AIR_BASE_PRESSURE)/GRAV_ACC))/ClimateUtils::GetAirDensity(pAir->m_temperature);
				cell.AddContent(0, pAir);

				// ground
				GroundContent* pGround = new GroundContent();
				pGround->m_maxWaterCapacity = GROUND_WATER_METERS;
				pGround->m_temperature = BASE_TEMPERATURE;
				cell.AddContent(1, pGround);
			}
			
			if(pAir)
			{
				UpdateContent(*pAir);
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
	//m_sunStep = map.GetSizeX() / DAYLONG;
	m_sunStep = 1;
	m_timeStep = (DAYLONG*TIME_STEP)/(float)m_map.GetSizeY();
	
}

void ClimateGenerator::SimulateClimateStep()
{
	SunHeatingStep();
	
	OneCellStep();
	
	AirMoveStep();
	
	WaterMoveStep();
	
	//AirMoveHighStep();
	//UpdateStep();
	//
	//AirMoveLowStep();
	
	UpdateStep();
		
	//add sun step by one hour
	//m_sunPosX = (m_sunPosX + m_sunStep) % m_climateMap.GetSizeX();
	m_sunPosX = ((m_sunPosX - m_sunStep) > 0) ? (m_sunPosX - m_sunStep) : (m_sunPosX - m_sunStep) + m_climateMap.GetSizeX();
}


void ClimateGenerator::SunHeatingStep()
{
	
	static const float degreeStep = PI / m_map.GetSizeY();
	//static const float clA = 1/(MEASURE_HEIGHT - CLOADS_UNDER_HEIGHT);
	//static const float clB = -CLOADS_UNDER_HEIGHT/(MEASURE_HEIGHT - CLOADS_UNDER_HEIGHT);
	
	float degreeY = - (PI / 2);
	float cosY;
	
	float totalEnergy = 0;
	float totalEnergyGround = 0;
	
	int32_t level = 0;
	
	m_oneStepHeat = 0;
	
	// add warm to the surface
	for(int32_t y = m_sunPosY - (m_map.GetSizeY() / 2); y < m_sunPosY + (m_map.GetSizeY() / 2); ++y)
	{
		float degreeX = - (PI / 2);
		cosY = std::abs(std::cos(degreeY));
		for(int32_t x = m_sunPosX - (m_map.GetSizeY() / 2); x < m_sunPosX + (m_map.GetSizeY() / 2); ++x)
		{
			ClimateCell& cell = m_climateMap.GetCellValue(x,y);
			totalEnergyGround = totalEnergy = SUN_POWER * cosY * std::abs(std::cos(degreeX)) * m_timeStep; // energy per time step
			
			level = 0;
			if(cell.IsCheckContent(level, CellContent::AIR))
			{
				AirContent &air = *cell.GetAirContent(level);
				AddHeat(air, (totalEnergy * ATHMOSPHERIC_ABSORBTION));
				
				// is clouds
				if(air.m_cloudsHeight < MEASURE_HEIGHT)
				{
					//float ratio = clA*air.m_cloudsHeight + clB;
					//ratio = (ratio < 1) ? ratio : 1;
					totalEnergyGround -= (totalEnergy * CLOUDS_SCATTERED);
				}
			}
			m_oneStepHeat += totalEnergyGround;
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
	
	//std::cout << " Total Energy: " << m_oneStepHeat << std::endl;
	m_oneStepHeat = -m_oneStepHeat / (float)(m_climateMap.GetSizeY()*m_climateMap.GetSizeX());
}

void ClimateGenerator::Cooling(ClimateCell& cell)
{
	float energyLoss = 0.0;
	AirContent *pAir = nullptr;
	GroundContent *pGround = nullptr;
	WaterContent *pLowWater = nullptr;
	WaterContent *pDeepWater = nullptr;
	
	if(cell.IsCheckContent(0, CellContent::AIR))
	{
		pAir = cell.GetAirContent(0);
		
		//energyLoss = ClimateUtils::GetThermalConductivity(CellContent::AIR) 
		//		* ( 173 - ClimateUtils::CompRealAirTemp(pAir->m_temperature, pAir->m_baseAltitude, 11000)) * m_timeStep; // 
		
		AddHeat(*pAir, m_oneStepHeat);
	}
	
	if(cell.IsCheckContent(1, CellContent::GROUND))
	{
		pGround = cell.GetGroundContent(1);
		
		energyLoss = ClimateUtils::GetThermalConductivity(CellContent::GROUND) * ( pAir->m_temperature - pGround->m_temperature ) * m_timeStep;

		AddHeat(*pAir, -energyLoss);
		AddHeat(*pGround, energyLoss);
		
	}
	
	if(cell.IsCheckContent(1, CellContent::WATER))
	{
		pLowWater = cell.GetWaterContent(1);
		
		energyLoss = ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( pAir->m_temperature - pLowWater->m_temperature ) * m_timeStep;

		AddHeat(*pAir, -energyLoss);
		AddHeat(*pLowWater, energyLoss);
		
	}
	
	if(cell.IsCheckContent(2, CellContent::WATER))
	{
		pDeepWater = cell.GetWaterContent(2);
		
		energyLoss = ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( pLowWater->m_temperature - pDeepWater->m_temperature ) * m_timeStep;

		AddHeat(*pLowWater, -energyLoss);
		AddHeat(*pDeepWater, energyLoss);
	}
	
}

void ClimateGenerator::Precipitation(ClimateCell& cell)
{
	AirContent &air = *cell.GetAirContent(0);
	
	
	if(air.m_cloudsHeight < RAIN_HEIGHT)
	{
		//float mass = CompAirMass(air, air.m_cloudsHeight);
		float mass = CompAirMass(air, air.m_baseAltitude + air.m_cloudsHeight);
		float waterMass = mass*air.m_actHum;
		float waterTemp = ClimateUtils::CompRealAirTemp(air.m_temperature, air.m_baseAltitude, air.m_cloudsHeight+air.m_baseAltitude);
		
		waterMass = (waterMass > air.m_waterMass) ? air.m_waterMass : waterMass;
		
		if(waterTemp < 0)
		{
			air.m_precip = CellContent::SNOW;
		} else {
			air.m_precip = CellContent::WATER;
		}
		
		air.m_waterMassDiff -= waterMass;
		
		if(cell.IsCheckContent(1,CellContent::WATER))
		{
			WaterContent &water = *cell.GetWaterContent(1);
			float wMass = CompWaterMass(water);
			float temp = ((waterMass*waterTemp) + (wMass*water.m_temperature))/(wMass + waterMass);
			float Q = waterMass*(waterTemp - temp)*ClimateUtils::GetSpecificHeat(CellContent::WATER);
			AddHeat(water, Q);
			water.m_waterMassDiff += waterMass;
			
		} else if(cell.IsCheckContent(1, CellContent::GROUND)) {
			GroundContent &ground = *cell.GetGroundContent(1);
			float gMass = CompGroundMass(ground);
			float temp = ((waterMass*waterTemp*ClimateUtils::GetSpecificHeat(CellContent::WATER)) + (gMass*ground.m_temperature*ClimateUtils::GetSpecificHeat(CellContent::GROUND)))
						/(gMass*ClimateUtils::GetSpecificHeat(CellContent::GROUND) + waterMass*ClimateUtils::GetSpecificHeat(CellContent::WATER));
			float Q = waterMass*(waterTemp - temp)*ClimateUtils::GetSpecificHeat(CellContent::WATER);
			AddHeat(ground, Q);
			ground.m_waterMassDiff += waterMass;
		}
		
	} else {
		air.m_precip = CellContent::NONE;
	}
}


void ClimateGenerator::OneCellStep()
{
	for(int32_t y = 0; y < m_map.GetSizeY(); ++y)
	{
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x)
		{
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			AirContent &air = *cell.GetAirContent(0);
			// cooling
			Cooling(cell);
			
			Evaporation(cell);
			
			Precipitation(cell);
			
			UpdateContent(air);
			if(cell.IsCheckContent(1,CellContent::WATER))
			{
				UpdateContent(*cell.GetWaterContent(1));
			} else {
				UpdateContent(*cell.GetGroundContent(1));
			}
			
		}
	}
}

void ClimateGenerator::Evaporation(ClimateCell& cell)
{
	// bad - too much computing...:) 
	
	AirContent &air = *cell.GetAirContent(0);
	
	CellContent::ContentType content = cell.GetContent(1)->GetContentType();
	
	switch(content)
	{
	case CellContent::WATER:
	{
		WaterContent &water = *cell.GetWaterContent(1);
			
		if(water.m_temperature < WATER_FREEZING)
		{
			return;
		}
		
		float maxHum = ClimateUtils::CompMaxAirHumidity((float)air.m_temperature, air.m_airPressure);
		
		if(maxHum < air.m_actHum)
		{
			return;
		}
		
		float lenght = air.m_lowWind.Length();
		//float waterMassEvap = (25.0 + 19.0*lenght)*(maxHum - air.m_actHum);
		float waterMassEvap = (25.0 + 9.0*lenght)*(maxHum - air.m_actHum);
		float heat = HEAT_OF_VAPORIZATION_OF_WATER * waterMassEvap;
		float heat2air = waterMassEvap*ClimateUtils::GetSpecificHeat(CellContent::AIR)*(water.m_temperature - air.m_temperature);
		
		if(waterMassEvap < 0)
		{
			std::cout << "Evaporation failed..." << std::endl;
		}
		
		water.m_waterMassDiff -= waterMassEvap;
		air.m_waterMassDiff += waterMassEvap;
		
		AddHeat(water, -heat);
		AddHeat(air, heat2air);
	} 
		break;
	case CellContent::GROUND:
	{
		GroundContent &ground = *cell.GetGroundContent(1);
		
		if((ground.m_temperature < WATER_FREEZING)||(ground.m_waterMass <= 0.0))
		{
			return;
		}
		
		float maxHum = ClimateUtils::CompMaxAirHumidity((float)air.m_temperature, air.m_airPressure);
		
		if(maxHum < air.m_actHum)
		{
			return;
		}
				
		float lenght = air.m_lowWind.Length();
		//float waterMassEvap = (25.0 + 19.0*lenght)*(maxHum - air.m_actHum);
		float waterMassEvap = (25.0 + 9.0*lenght)*(maxHum - air.m_actHum);
		waterMassEvap = (ground.m_waterMass > waterMassEvap) ? waterMassEvap : ground.m_waterMass;
		
		float heat = HEAT_OF_VAPORIZATION_OF_WATER * waterMassEvap;
		float heat2air = waterMassEvap*ClimateUtils::GetSpecificHeat(CellContent::AIR)*(ground.m_temperature - air.m_temperature);
		
		if(waterMassEvap < 0)
		{
			std::cout << "Evaporation failed..." << std::endl;
		}
		
		ground.m_waterMassDiff -= waterMassEvap;
		air.m_waterMassDiff += waterMassEvap;
		
		AddHeat(ground, -heat);
		AddHeat(air, heat2air);
		
	}
		break;
	default:
		break;
	}
}



void ClimateGenerator::AirForceComp(AirContent& air, int32_t x, int32_t y, float sinLatitude)
{
	//static const float nS = SQUARE_SIDE_SIZE / 2; //neightbour surface
	static const float nS = 0.5; //neightbour surface
	
	int32_t nx, ny;
	Urho3D::Vector2 topDiff;
	Urho3D::Vector2 lowDiff;
	
	air.m_maxPressureDiff = 0;
	air.m_maxVolumeDiff = 0;
	air.m_lowForce = Urho3D::Vector2::ZERO;
	//air.m_highForce = Urho3D::Vector2::ZERO;
	
	// density in high * GRAV_ACC * nS
	float dens = CompDensity(air);
	//float densHigh = CompDensityInAltitude(air, air.m_baseAltitude + MEASURE_HEIGHT);
	
	//float highConst = densHigh*GRAV_ACC*nS;
	
	float compPressureLow = air.m_airPressure - air.m_dynamicPressureLow;
	

	float dp;
	float lowForce;
	//float highForce;
			
	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
	{
		
		m_climateMap.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), x, y, nx, ny);
		
		AirContent &nAir = *m_climateMap.GetCellValue(nx,ny).GetAirContent(0);
		
		{
			dp = compPressureLow - nAir.m_airPressure + nAir.m_dynamicPressureLow;
			lowForce = dp*nS;
			
			//highForce = (air.m_volumeLevel > nAir.m_volumeLevel) ? (air.m_volumeLevel - nAir.m_volumeLevel)*highConst 
			//				: CompDensityInAltitude(nAir, air.m_baseAltitude + MEASURE_HEIGHT)*GRAV_ACC*nS*(air.m_volumeLevel - nAir.m_volumeLevel);
			
			if(dp > air.m_maxPressureDiff)
			{
				air.m_maxPressureDiff = dp;
			}
			
			if((air.m_volumeLevel - nAir.m_volumeLevel) > air.m_maxVolumeDiff)
			{
				air.m_maxVolumeDiff = (air.m_volumeLevel - nAir.m_volumeLevel);
			}
			
			if(std::isinf(lowForce)||std::isnan(lowForce))
			{
				std::cout << "Error! broken force: " << lowForce << " at " << x << ", " << y << std::endl;
			}
			
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
			
			//vec = vec*force;
			//lowForce /= m_timeStep;
			//highForce /= m_timeStep;
			
			air.m_lowForce += vec*lowForce;
			//air.m_highForce += vec*highForce;
			
			// dynamic pressure comp
			air.m_lowForce += nAir.m_lowWind.Normalized()*nAir.m_dynamicPressureLow*nS*0.2;
			//air.m_highForce += nAir.m_highWind.Normalized()*nAir.m_dynamicPressureHigh*nS;

		}
		
	}
	

	
	// Coriolis effect 
	if(true)
	{
		//Coriolis effect
		float f = 2*ANGULAR_VELOCITY*sinLatitude;
		
		Urho3D::Vector2 Fc = Urho3D::Vector2(air.m_lowWind.y_, -air.m_lowWind.x_) * f;
		air.m_lowForce += Fc;
		
		//Fc = Urho3D::Vector2(air.m_highWind.y_, -air.m_highWind.x_) * f;
		//air.m_highForce += Fc;
	}
	
	// drag
	if(true)
	{
		float dragConst = 0.5; //0.5*nS;
		
		// low wind
		float windSpeed = air.m_lowWind.Length();
		float scalFd = dens*dragConst*windSpeed*windSpeed;
		Urho3D::Vector2 Fd = air.m_lowWind.Normalized()*(-scalFd);
		air.m_lowForce += Fd;
		
		// high wind
		//windSpeed = air.m_highWind.Length();
		//scalFd = densHigh*dragConst*windSpeed*windSpeed;
		//Fd = air.m_highWind.Normalized()*(-scalFd);
		//air.m_highForce += Fd;
		
		
	}
	
	//air.m_lowForce /= m_timeStep;
	//air.m_highForce /= m_timeStep;
	
	//if(air.m_lowForce.Length() > FORCE_SIZE_PROTECTION)
	//{
	//	//std::cout << "Warning! suspicious force: " << air.m_lowForce.Length() << " at " << x << ", " << y << std::endl;
	//	air.m_lowForce = air.m_lowForce.Normalized()*FORCE_SIZE_PROTECTION;
	//}
	//
	//
	//if(air.m_highForce.Length() > FORCE_SIZE_PROTECTION)
	//{
	//	//std::cout << "Warning! suspicious force: " << air.m_highForce.Length() << " at " << x << ", " << y << std::endl;
	//	air.m_highForce = air.m_highForce.Normalized()*FORCE_SIZE_PROTECTION;
	//}
	
	
}



void ClimateGenerator::AirLowWindMassChange(AirContent& source, int32_t x, int32_t y)
{
	static const float ratioT[2][2] = {{0.32325, 0.3535}, {0.17675,0.6465}};
	
	float dens = CompDensity(source);
	
	// force impuls length
	//float timeDiff = 0.1*std::sqrt(source.m_maxPressureDiff/dens*SQUARE_SIDE_SIZE);
	//timeDiff = (timeDiff > m_timeStep) ? m_timeStep : timeDiff;
	
	// mass from ground to MEASURE_HEIGHT
	//float massSource = source.m_airMass*(1 - ClimateUtils::CompAbsolutePressure(source.m_baseAltitude + MEASURE_HEIGHT, source.m_airPressure)/source.m_airPressure);
	
	float massSource = (source.m_maxPressureDiff/GRAV_ACC)*0.5;
	
	// wind speed change
	Urho3D::Vector2 windChange = (source.m_lowForce*m_timeStep)/(dens*SQUARE_SIZE);
	source.m_lowWind = source.m_lowWind + windChange;
	
	// neightbour coords
	int32_t nx, ny;
	// get receiver
	float speed = source.m_lowWind.Length();
	AirContent* pDest = nullptr;
	int32_t mainDir;
	float waterMass;
	
	if(massSource > (MASS_MOVE_PROTECTION*source.m_airMass))
	{
		//std::cout << "Warning! critical mass move (low): " << massSource << " out of " << source.m_airMass << " at " << x << ", " << y << std::endl;
		massSource = MASS_MOVE_PROTECTION*source.m_airMass;
	}
	
	// limit wind speed
	if(speed > WIND_SPEED_PROTECTION)
	{
		//std::cout << "Warning! speed fail (low): " << speed << " at " << x << ", " << y << std::endl;
		source.m_lowWind = source.m_lowWind.Normalized() * WIND_SPEED_PROTECTION;
	}
	
	if(std::isinf(speed)||std::isnan(speed))
	{
		std::cout << "Error! speed fail (low): " << speed << " at " << x << ", " << y << std::endl;
		return;
	}
	
	
	
	if(speed > 0)
	{
		// make it easier
		float volumeRatio = ((speed*m_timeStep*SQUARE_SIDE_SIZE)/SQUARE_SIZE);
		volumeRatio = (volumeRatio <= 1)? volumeRatio : 1;
		
		float massChange = volumeRatio*massSource; 
		
		if(source.m_cloudsHeight < (MEASURE_HEIGHT))
		{
			// all cloud is pushed away
			waterMass = (CompAirMass(source, source.m_cloudsHeight)*source.m_actHum)*volumeRatio;
		} else {
			// only humidity is moved
			waterMass = source.m_actHum*massChange; // water mass change per square meter
		}
		
		waterMass = (waterMass < (source.m_waterMass/2)) ? waterMass : (source.m_waterMass/2);
		
		if(source.m_airPressure > MIN_AIR_PRESSURE_PROTECTION)
		{
			source.m_airMassDiff -= massChange;
		} else {
			massChange = 0;
		}
		source.m_waterMassDiff -= waterMass;
		
		// get destinations
		mainDir = (int32_t)GetWindDest(source.m_lowWind);
		mainDir = (mainDir + 7) % 8;
		const float *ratio = ratioT[(mainDir % 2)];
		float heatChange = 0;
		
		bool maxPressProt = false;
		
		for(int32_t i = 0; i < 3; ++i)
		{
			m_climateMap.GetNeightbourCoords((MapContainerDIR)mainDir, nx, ny);
			nx += x;
			ny += y;
			pDest = m_climateMap.GetCellValue(nx, ny).GetAirContent(0);
			
			maxPressProt = (pDest->m_airPressure > MAX_AIR_PRESSURE_PROTECTION);
			
			float r = ratio[(i%2)];
			
			if(!maxPressProt)
			{
				heatChange = massChange*r*ClimateUtils::GetSpecificHeat(CellContent::AIR)*
				(ClimateUtils::CompRealAirTemp(source.m_temperature, source.m_baseAltitude, pDest->m_baseAltitude) - pDest->m_temperature);
			} else {
				heatChange = 0;
			}
			
			if(std::isnan(heatChange))
			{
				std::cout << "AirWindMassChange Error" << std::endl;
			}
			
			AddHeat(*pDest, heatChange);
			
			if(!maxPressProt)
			{
				pDest->m_airMassDiff += massChange*r;
			} else {
				// return air mass to source
				source.m_airMassDiff += massChange*r;
			}
			pDest->m_waterMassDiff += waterMass*r;
						
			mainDir = (mainDir + 1) % 8;
		}
	}
}

/*
void ClimateGenerator::AirHighWindMassChange(AirContent& source, int32_t x, int32_t y)
{
	static const float ratioT[2][2] = {{0.32325, 0.3535}, {0.17675,0.6465}};
	
	float dens = CompDensityInAltitude(source, source.m_baseAltitude + MEASURE_HEIGHT);
	
	// force impuls length - time to lost half of volume diffence for zero speed
	//float timeDiff = std::sqrt((2*source.m_maxVolumeDiff)/(SQUARE_SIDE_SIZE*GRAV_ACC));
	//timeDiff = (timeDiff > m_timeStep) ? m_timeStep : timeDiff;
	
	// mass from ground to MEASURE_HEIGHT
	//float massSource = source.m_airMass*(1 - ClimateUtils::CompAbsolutePressure(source.m_baseAltitude + MEASURE_HEIGHT, source.m_airPressure)/source.m_airPressure);
	
	float massSource = (source.m_maxVolumeDiff * dens)*0.5;
	
	// wind speed change
	Urho3D::Vector2 windChange = (source.m_highForce*m_timeStep)/(dens*SQUARE_SIZE);
	source.m_highWind = source.m_highWind + windChange;
	
	// neightbour coords
	int32_t nx, ny;
	// get receiver
	float speed = source.m_highWind.Length();
	AirContent* pDest = nullptr;
	int32_t mainDir;
	float waterMass;
	

	// simulation protection
	if(massSource > (MASS_MOVE_PROTECTION*source.m_airMass))
	{
		//std::cout << "Warning! critical mass move (high): " << massSource << " out of " << source.m_airMass << " at " << x << ", " << y << std::endl;
		massSource = MASS_MOVE_PROTECTION*source.m_airMass;
	}
	
	// limit wind speed
	if(speed > WIND_SPEED_PROTECTION)
	{
		//std::cout << "Warning! speed fail (high): " << speed << " at " << x << ", " << y << std::endl;
		source.m_highWind = source.m_highWind.Normalized() * WIND_SPEED_PROTECTION;
	}
	
	if(std::isinf(speed)||std::isnan(speed))
	{
		std::cout << "Error! speed fail: " << speed << " at " << x << ", " << y << std::endl;
		return;
	}
	
	
	if(speed > 0)
	{
		// make it easier
		float volumeRatio = ((speed*m_timeStep*SQUARE_SIDE_SIZE)/SQUARE_SIZE);
		volumeRatio = (volumeRatio <= 1)? volumeRatio : 1;
		
		float massChange = volumeRatio*massSource; 
		
		//if(source.m_cloudsHeight < (MEASURE_HEIGHT+MEASURE_HEIGHT))
		//{
		//	// all cloud is pushed away
		//	waterMass = (CompAirMass(source, source.m_cloudsHeight)*source.m_actHum)*volumeRatio;
		//} else {
			// only humidity is moved
		waterMass = source.m_actHum*massChange; // water mass change per square meter
		//}
		
		waterMass = (waterMass < (source.m_waterMass/2)) ? waterMass : (source.m_waterMass/2);
		
		if(source.m_airPressure > MIN_AIR_PRESSURE_PROTECTION)
		{
			source.m_airMassDiff -= massChange;
		} else {
			massChange = 0;
		}
		source.m_waterMassDiff -= waterMass;
		
		// get destinations
		mainDir = (int32_t)GetWindDest(source.m_highWind);
		mainDir = (mainDir + 7) % 8;
		const float *ratio = ratioT[(mainDir % 2)];
		float heatChange;
		bool maxPressProt = false;
		
		for(int32_t i = 0; i < 3; ++i)
		{
			m_climateMap.GetNeightbourCoords((MapContainerDIR)mainDir, nx, ny);
			nx += x;
			ny += y;
			pDest = m_climateMap.GetCellValue(nx, ny).GetAirContent(0);
			
			maxPressProt = (pDest->m_airPressure > MAX_AIR_PRESSURE_PROTECTION);
			
			float r = ratio[(i%2)];
			
			if(!maxPressProt)
			{
				heatChange = massChange*r*ClimateUtils::GetSpecificHeat(CellContent::AIR)*
				(ClimateUtils::CompRealAirTemp(source.m_temperature, source.m_baseAltitude, pDest->m_baseAltitude) - pDest->m_temperature);
			} else {
				heatChange = 0;
			}
			
			if(std::isnan(heatChange))
			{
				std::cout << "AirWindMassChange Error" << std::endl;
			}
			
			AddHeat(*pDest, heatChange);
			
			if(!maxPressProt)
			{
				pDest->m_airMassDiff += massChange*r;
			} else {
				// return to source
				source.m_airMassDiff += massChange*r;
			}
			pDest->m_waterMassDiff += waterMass*r;
						
			mainDir = (mainDir + 1) % 8;
		}
	}
}
*/

void ClimateGenerator::AirHighWindMassChange(AirContent& source, int32_t x, int32_t y)
{
	
	float dens = CompDensityInAltitude(source, source.m_baseAltitude + MEASURE_HEIGHT);
	
	//float massSource = (source.m_maxVolumeDiff * dens);
	
	// wind speed change
	//Urho3D::Vector2 windChange = (source.m_highForce*timeDiff)/(dens*SQUARE_SIZE);
	//source.m_highWind = source.m_highWind + windChange;
	
	// neightbour coords
	int32_t nx, ny;
	// get receiver
	//float speed = source.m_highWind.Length();
	AirContent* destList[8];
	AirContent* pDest = nullptr;
	int32_t count = 0;

	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
	{
		
		m_climateMap.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), x, y, nx, ny);
		
		pDest = m_climateMap.GetCellValue(nx,ny).GetAirContent(0);
		
		if(pDest->m_volumeLevel < source.m_volumeLevel)
		{
			destList[count] = pDest;
			++count;
			// insert sort
			for(int32_t i = (count-1); i > 0; --i)
			{
				if(destList[i-1]->m_volumeLevel < destList[i]->m_volumeLevel)
				{
					pDest = destList[i];
					destList[i] = destList[i-1];
					destList[i-1] = pDest;
				} else {
					break;
				}
			}
		}
	}
	
	float airMass = 0;
	float waterMass = 0;
	float volumeLevel = source.m_volumeLevel;
	
	float changeAir = 0;
	float changeWater = 0;
	
	for(int32_t i = 0; i < count; ++i)
	{
		if(volumeLevel > destList[i]->m_volumeLevel)
		{
			changeAir = (volumeLevel - destList[i]->m_volumeLevel)*0.5; //air mass
		
			volumeLevel -= changeAir;
			
			changeAir *= dens;
			
			changeWater = changeAir*source.m_actHum;
			
			airMass += changeAir;
			waterMass += changeWater;
			
			destList[i]->m_airMassDiff += changeAir;
			destList[i]->m_waterMassDiff += changeWater;
		
		} else {
			break; // end
		}
	}
	
	source.m_airMassDiff -= airMass;
	source.m_waterMassDiff -= waterMass;
	
}


MapContainerDIR ClimateGenerator::GetWindDest(Urho3D::Vector2& windVec)
{
	float angle = windVec.Angle(Urho3D::Vector2(0.0, -1.0));
	
	if(angle < 22.5)
	{
		return MapContainerDIR::N;
	} else if (angle < 67.5) {
		if(windVec.x_ > 0) return MapContainerDIR::NE;
		else return MapContainerDIR::NW;
	} else if (angle < 112.5) {
		if(windVec.x_ > 0) return MapContainerDIR::E;
		else return MapContainerDIR::W;
	} else if (angle < 157.5) {
		if(windVec.x_ > 0) return MapContainerDIR::SE;
		else return MapContainerDIR::SW;
	} else {
		return MapContainerDIR::S;		
	}
}

void ClimateGenerator::UpdateStep()
{
	for(int32_t y = 0; y < m_map.GetSizeY(); ++y)
	{
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x)
		{
			
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			AirContent &air = *cell.GetAirContent(0);
			
			if(!UpdateContent(air))
			{
				std::cout << " Error cell: " << x << ", " << y << std::endl;
			}
			
			if(cell.IsCheckContent(1,CellContent::WATER))
			{
				UpdateContent(*cell.GetWaterContent(1));
			} else {
				UpdateContent(*cell.GetGroundContent(1));
			}
		}
	}
}


void ClimateGenerator::AirMoveStep()
{
	static const float degreeStep = PI / m_climateMap.GetSizeY();
	float degreeY = - (PI / 2);
	float sinLatitude;
	
	for(int32_t y = 0; y < m_map.GetSizeY(); ++y)
	{
		sinLatitude = std::sin(degreeY);
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x)
		{
			
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			AirContent &air = *cell.GetAirContent(0);
			
			AirForceComp(air, x,y, sinLatitude);
			
			AirHighWindMassChange(air, x, y);
			//AirHighWindMassChange2(air, x, y);
			
			AirLowWindMassChange(air, x, y);
			
		}
		
		degreeY += degreeStep;			
	}
}

void ClimateGenerator::WaterForceComp(WaterContent& water, int32_t x, int32_t y, float sinLatitude)
{
	static const float nS = SQUARE_SIDE_SIZE / 2; //neightbour surface
	
	int32_t nx, ny;
	
	water.m_force = Urho3D::Vector2::ZERO;
	water.m_maxSurfaceDiff = 0;
	
	float force;
	float c = ClimateUtils::WATER_DENSITY*GRAV_ACC*nS;
	
	if(water.m_waterMass <= 0)
	{
		// no move anymore
		return;
	}
			
	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
	{
		
		m_climateMap.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), x, y, nx, ny);
		
		ClimateCell &nCell = m_climateMap.GetCellValue(nx,ny);
		
		if(nCell.IsCheckContent(1, CellContent::WATER))
		{
			WaterContent &wc = *nCell.GetWaterContent(1);
			
			force = (water.m_surfaceLevel - wc.m_surfaceLevel)*c;
			
			if(water.m_maxSurfaceDiff < (water.m_surfaceLevel - wc.m_surfaceLevel))
			{
				water.m_maxSurfaceDiff = (water.m_surfaceLevel - wc.m_surfaceLevel);
			}
			
			if(std::isinf(force)||std::isnan(force))
			{
				std::cout << "Error! broken force: " << force << " at " << x << ", " << y << std::endl;
			}
			
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
			
			//vec = vec*force;
			water.m_force += vec*force;
			
			// dynamic pressure comp
			water.m_force += wc.m_streamDir.Normalized()*wc.m_dynamicPressure*nS;

		}
		
	}
	
	// Coriolis effect 
	if(true)
	{
		//Coriolis effect
		float f = 2*ANGULAR_VELOCITY*sinLatitude;
		
		Urho3D::Vector2 Fc = Urho3D::Vector2(water.m_streamDir.y_, -water.m_streamDir.x_) * f;
		water.m_force += Fc;
		
	}
	
	// drag
	if(true)
	{
		float dragConst = 0.5*nS;
		
		// low wind
		float windSpeed = water.m_streamDir.Length();
		float scalFd = ClimateUtils::WATER_DENSITY*dragConst*windSpeed*windSpeed;
		Urho3D::Vector2 Fd = water.m_streamDir.Normalized()*(-scalFd);
		water.m_force += Fd;
		
	}
	
	//if(air.m_lowForce.Length() > FORCE_SIZE_PROTECTION)
	//{
	//	//std::cout << "Warning! suspicious force: " << air.m_lowForce.Length() << " at " << x << ", " << y << std::endl;
	//	air.m_lowForce = air.m_lowForce.Normalized()*FORCE_SIZE_PROTECTION;
	//}
	//
	//if(air.m_highForce.Length() > FORCE_SIZE_PROTECTION)
	//{
	//	//std::cout << "Warning! suspicious force: " << air.m_highForce.Length() << " at " << x << ", " << y << std::endl;
	//	air.m_highForce = air.m_highForce.Normalized()*FORCE_SIZE_PROTECTION;
	//}
}

void ClimateGenerator::WaterStreamMassChange(WaterContent& source, int32_t x, int32_t y)
{
	static const float ratioT[2][2] = {{0.32325, 0.3535}, {0.17675,0.6465}};
	
	// force impuls length - time to lost half of volume diffence for zero speed
	float timeDiff = std::sqrt((2*source.m_maxSurfaceDiff)/(SQUARE_SIDE_SIZE*GRAV_ACC));
	timeDiff = (timeDiff > m_timeStep) ? m_timeStep : timeDiff;
	
	// mass from ground to MEASURE_HEIGHT
	//float massSource = source.m_airMass*(1 - ClimateUtils::CompAbsolutePressure(source.m_baseAltitude + MEASURE_HEIGHT, source.m_airPressure)/source.m_airPressure);
	
	float massSource = (source.m_maxSurfaceDiff * ClimateUtils::WATER_DENSITY)*0.5;
	
	// wind speed change
	Urho3D::Vector2 streamChange = (source.m_force*timeDiff)/(ClimateUtils::WATER_DENSITY*SQUARE_SIZE);
	source.m_streamDir = source.m_streamDir + streamChange;
	
	// neightbour coords
	int32_t nx, ny;
	// get receiver
	float speed = source.m_streamDir.Length();
	WaterContent* pDest = nullptr;
	int32_t mainDir;
	
	if(source.m_waterMass <= 0)
	{
		//no move anymore
		source.m_streamDir = Urho3D::Vector2::ZERO;
		return;
	}

	// simulation protection
	if(massSource > (MASS_MOVE_PROTECTION*source.m_waterMass))
	{
		//std::cout << "Warning! critical mass move (high): " << massSource << " out of " << source.m_airMass << " at " << x << ", " << y << std::endl;
		massSource = MASS_MOVE_PROTECTION*source.m_waterMass;
	}
	
	// limit wind speed
	if(speed > WIND_SPEED_PROTECTION)
	{
		//std::cout << "Warning! speed fail (high): " << speed << " at " << x << ", " << y << std::endl;
		source.m_streamDir = source.m_streamDir.Normalized() * WIND_SPEED_PROTECTION;
	}
	
	if(std::isinf(speed)||std::isnan(speed))
	{
		std::cout << "Error! water stream speed fail: " << speed << " at " << x << ", " << y << std::endl;
		return;
	}
	
	
	if(speed > 0)
	{
		// make it easier
		float volumeRatio = ((speed*m_timeStep*SQUARE_SIDE_SIZE)/SQUARE_SIZE);
		volumeRatio = (volumeRatio <= 1)? volumeRatio : 1;
		
		float massChange = volumeRatio*massSource; 
		
		source.m_waterMassDiff -= massChange;
		
		// get destinations
		mainDir = (int32_t)GetWindDest(source.m_streamDir);
		mainDir = (mainDir + 7) % 8;
		const float *ratio = ratioT[(mainDir % 2)];
		
		for(int32_t i = 0; i < 3; ++i)
		{
			m_climateMap.GetNeightbourCoords((MapContainerDIR)mainDir, nx, ny);
			nx += x;
			ny += y;
			if(m_climateMap.GetCellValue(nx,ny).IsCheckContent(1, CellContent::WATER))
			{
				pDest = m_climateMap.GetCellValue(nx, ny).GetWaterContent(1);
				
				float r = ratio[(i%2)];
				
				float heatChange = massChange*r*ClimateUtils::GetSpecificHeat(CellContent::WATER)*(source.m_temperature - pDest->m_temperature);
				
				if(std::isnan(heatChange))
				{
					std::cout << "water heatChange Error" << std::endl;
				}
				
				AddHeat(*pDest, heatChange);
				
				pDest->m_waterMassDiff += massChange*r;
			
			}
						
			mainDir = (mainDir + 1) % 8;
		}
	}
}

void ClimateGenerator::WaterMoveStep()
{
	static const float degreeStep = PI / m_climateMap.GetSizeY();
	float degreeY = - (PI / 2);
	float sinLatitude;
	
	for(int32_t y = 0; y < m_map.GetSizeY(); ++y)
	{
		sinLatitude = std::sin(degreeY);
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x)
		{
			
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			if(cell.IsCheckContent(1, CellContent::WATER))
			{
				WaterContent &water = *cell.GetWaterContent(1);
				
				WaterForceComp(water, x,y, sinLatitude);
				
				WaterStreamMassChange(water, x, y);
				
			}
			
		}
		
		degreeY += degreeStep;			
	}
}










