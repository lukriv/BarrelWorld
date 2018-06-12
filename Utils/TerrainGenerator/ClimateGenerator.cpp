#include "ClimateGenerator.h"

#include <iostream>
#include <sstream>

#include <cmath>
#include "ClimateUtils.h"
#include "utils.h"

#define PI 3.14159265

const float WATER_SPECIFIC = 334000; // [J] merne skupenske teplo tani vody

// parameters
const float GROUND_WATER_CAPACITY = 1000; // in kg - defines when the water begin to flow out
const float GROUND_HEAT_METERS = 3;
const float GROUND_WATER_RUNOFF = 0.1;

const float ATHMOSPHERIC_REFLECTION = 0.3;
const float ATHMOSPHERIC_ABSORBTION =  	0.175; //
const float CLOUDS_SCATTERED =  0.145; //
const float GROUND_REFLECTION = 0.04;

const float SUN_POWER = 1400; // power [W/m^2]
const float REAL_SUN_POWER = (1 - ATHMOSPHERIC_REFLECTION)*SUN_POWER;


const float GRAV_ACC = 10;

const float LOW_WATER_METERS = 3; // deep of low water

const float SQUARE_SIDE_SIZE = 10000;


const float MEASURE_HEIGHT = 3000;
const float VOLUME_HEIGHT = 10000;

const int32_t YEAR_LENGTH = 365; //days
const int32_t DAYLONG = 24; // hours
const float TIME_STEP = 3600; // time step in seconds - hour
const int32_t MAX_SUN_LATITUDE = 23; // degree

const float DEEP_WATER_BASE_TEMPERATURE = 2 + 273;
const float BASE_TEMPERATURE = 15 + 273;

const float PRESSURE_FACTOR = 0.5;
const float VOLUME_FACTOR = 0.6;


const float HEAT_TRANSFER_COEFICIENT_AIR = 20; // can be 5 - 37 W/(m^2*K) heat transfer coefficient for air
const float HEAT_TRANSFER_COEFICIENT_WATER = 200; // can be 100 - 1200 W/(m^2*K) heat transfer coefficient for water

// non parameters
const float SQUARE_SIZE = SQUARE_SIDE_SIZE* SQUARE_SIDE_SIZE;
const float nS = 0.5*SQUARE_SIDE_SIZE; // normalized surface for force computing

const float SALT_MASS_ON_SQUARE_METER = 35; // kilo per cubic meter


const float ANGULAR_VELOCITY = (2*PI)/(DAYLONG*TIME_STEP); // planet angular velocity

const float GROUND_HEIGHT_TO_METERS = 100;

const float AIR_BASE_PRESSURE = 101500.0;

//const float WATER_VISCOSITY = 0.000001; // [m^2/s]
//const float AIR_VISCOSITY = 0.0000133; // [m^2/s]
//const float AIR_VISCOSITY = 0.00001827; // [Pa*s]


//const float MASS_MOVE_PROTECTION = 0.05; // maximal mass move ( ratio to whole mass )
const float HIGH_WIND_SPEED_PROTECTION = 20; // maximal posible wind speed [m/s]
const float WIND_SPEED_PROTECTION = 10; // maximal posible wind speed [m/s]
//const float FORCE_SIZE_PROTECTION = 10000; // maximal posible force size [N]
//const float WIND_SPEED_CHANGE_PROTECTION = 10; // maximal posible wind speed change [m/s]

const float MAX_AIR_PRESSURE_PROTECTION = 107000; // maximal air pressure [Pa]
const float MIN_AIR_PRESSURE_PROTECTION = 87000; // minimal air pressure [Pa]

const float HEAT_OF_VAPORIZATION_OF_WATER = 2300000; // [ J/kg ]
const float ENTHALPY_OF_FUSION_OF_WATER = 335000; // [ J/kg ]

const float WATER_FREEZING = 273; // freeze temperature [ K ]
const float RAIN_HEIGHT = 1000; //
const float CLOADS_UNDER_HEIGHT = 5500;



static const Urho3D::Vector2 DirVector[8] = { {0,-1}  //N
	, { -0.7071, -0.7071} //NW
	, { -1, 0} 			// W
	, { -0.7071, 0.7071}	//SW
	, { 0, 1} 			//S
	, { 0.7071, 0.7071 } //SE
	, { 1, 0}			//E
	, { 0.7071, -0.7071}
}; // NE

static float VOLUME_BASE = 0;

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
	if(cont.m_baseAltitude == 0) {
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
	//return (  ClimateUtils::GROUND_DENSITY );
}

float CompWaterSalinity(WaterContent &cont)
{
	return (cont.m_saltMass / cont.m_waterMass) * ClimateUtils::WATER_DENSITY;
}

float CompWaterMass(WaterContent& cont)
{
	return  cont.m_iceMass + cont.m_waterMass + cont.m_saltMass;
	//return (cont.m_iceMass > ClimateUtils::ICE_DENSITY)? ClimateUtils::ICE_DENSITY : ClimateUtils::WATER_DENSITY;
}

void AddHeat(WaterContent &cont, float heat)
{
	if(std::isnan(heat)) {
		std::cout << "Error: Heat is nan" << std::endl;
		return;
	}

	if(heat > 0) {
		if(cont.m_iceMass == 0.0) {
			cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER));
		} else {
			if(cont.m_temperature >= WATER_FREEZING) {
				float iceMassDiff = heat / ENTHALPY_OF_FUSION_OF_WATER;
				if(cont.m_iceMass > iceMassDiff) {
					cont.m_waterMassDiff += iceMassDiff;
					cont.m_iceMassDiff -= iceMassDiff;
				} else {
					heat = heat - (cont.m_iceMass*ENTHALPY_OF_FUSION_OF_WATER);
					cont.m_waterMassDiff += cont.m_iceMass; // add water
					cont.m_iceMassDiff -= cont.m_iceMass; // no ice
					cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER)); // add rest of heat to temperature
				}
			} else {
				cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::ICE));
			}
		}
	} else {
		if(cont.m_waterMass > 0.0) {
			if(cont.m_temperature > WATER_FREEZING) {
				cont.m_temperatureDiff += heat / (CompWaterMass(cont) * ClimateUtils::GetSpecificHeat(CellContent::WATER));
			} else {
				float iceMassDiff = -(heat / ENTHALPY_OF_FUSION_OF_WATER);
				if(cont.m_waterMass > iceMassDiff) {
					cont.m_waterMassDiff -= iceMassDiff;
					cont.m_iceMassDiff += iceMassDiff;
				} else {
					heat = heat + (cont.m_waterMass*ENTHALPY_OF_FUSION_OF_WATER);
					cont.m_iceMassDiff += cont.m_waterMass;
					cont.m_waterMassDiff -= cont.m_waterMass;
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
	if((temp > 10)||(std::isnan(temp))||(std::isinf(temp)||(temp < -10))) {
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

	//cont.m_volumeLevel = (cont.m_airMass - ClimateUtils::CompAbsolutePressure(cont.m_baseAltitude + MEASURE_HEIGHT, cont.m_airPressure)/GRAV_ACC)/ClimateUtils::GetAirDensity(cont.m_temperature)*(cont.m_airPressure/AIR_BASE_PRESSURE)
	//					- cont.m_volumeBase;

	cont.m_volumeLevel = ((cont.m_airPressure - ClimateUtils::CompAbsolutePressure(VOLUME_HEIGHT, cont.m_airPressure))/GRAV_ACC)/(ClimateUtils::GetAirDensity(ClimateUtils::CompRealAirTemp(cont.m_temperature, cont.m_baseAltitude, 0))*(cont.m_airPressure/AIR_BASE_PRESSURE))
	                     - VOLUME_BASE;

	// air mass, temperature and air pressure must be updated already
	float windSpeed = cont.m_lowWind.Length();
	cont.m_dynamicPressureLow = 0.5*windSpeed*windSpeed*ClimateUtils::AIR_DENSITY*(cont.m_airPressure/AIR_BASE_PRESSURE);

	windSpeed = cont.m_highWind.Length();
	cont.m_dynamicPressureHigh = 0.5*windSpeed*windSpeed*CompDensityInAltitude(cont, VOLUME_HEIGHT);

	//
	cont.m_dynamicForce = cont.m_lowWind.Normalized()*cont.m_dynamicPressureLow*nS;
	cont.m_dynamicForceHigh = cont.m_highWind.Normalized()*cont.m_dynamicPressureHigh*nS;

	cont.m_actHum = cont.m_waterMass/cont.m_airMass;
	cont.m_actHum = (cont.m_actHum > 0) ? cont.m_actHum : 0; // water mass can be lower than zero

	cont.m_cloudsHeight = ClimateGenerator::CompCloudsHeight(cont);
	cont.m_cloudsHeight = (cont.m_cloudsHeight < 0) ? 0 : cont.m_cloudsHeight;

	//if((cont.m_airMass < 0)
	//   ||(cont.m_temperature < 0)) {
	//	std::stringstream sstr;
	//	sstr << "Error! Air: cont.m_airMass: " << cont.m_airMass
	//	     << " cont.m_temperature: " << cont.m_temperature << std::endl;
	//
	//	throw ClimateGenerator::ClimateException(sstr.str());
	//}

	cont.m_airMassDiff = 0.0;
	cont.m_temperatureDiff = 0.0;
	cont.m_waterMassDiff = 0.0;

	//if(cont.m_waterMass < 0) {
	//	std::cout << "Warning! Air: cont.m_waterMass: " << cont.m_waterMass << std::endl;
	//	retval = false;
	//}

	return retval;
}

bool UpdateContent(WaterContent &cont)
{
	bool retval = true;

	cont.m_temperature += cont.m_temperatureDiff;

	cont.m_waterMass += cont.m_waterMassDiff;

	cont.m_iceMass += cont.m_iceMassDiff;

	//cont.m_surfaceLevel = (cont.m_waterMass/ClimateUtils::WATER_DENSITY) + (cont.m_iceMass/ClimateUtils::ICE_DENSITY);

	//float speed = cont.m_streamDir.Length();
	//cont.m_dynamicPressure = 0.5*speed*speed*ClimateUtils::WATER_DENSITY;

	//cont.m_dynamicForce = cont.m_streamDir.Normalized()*cont.m_dynamicPressure*nS;


	cont.m_temperatureDiff = 0.0;
	cont.m_waterMassDiff = 0.0;
	cont.m_iceMassDiff = 0.0;

	//if(cont.m_temperature < 0) {
	//	std::stringstream sstr;
	//	sstr << "Error! Water: cont.m_waterMass: " << cont.m_waterMass
	//	     << " cont.m_iceMass: " << cont.m_iceMass
	//	     << " cont.m_temperature: " << cont.m_temperature << std::endl;
	//
	//	throw ClimateGenerator::ClimateException(sstr.str());
	//}
	
	//if((cont.m_waterMass + cont.m_iceMass) < 0) 
	//{
	//	std::stringstream sstr;
	//	sstr << "Error! Water: cont.m_waterMass: " << cont.m_waterMass
	//	     << " cont.m_iceMass: " << cont.m_iceMass
	//	     << " cont.m_temperature: " << cont.m_temperature << std::endl;
	//
	//	std::cout << sstr.str();
	//	
	//	return false;
	//}

	return retval;
}

bool UpdateContent(GroundContent &cont)
{
	bool retval = true;
	cont.m_temperature += cont.m_temperatureDiff;
	cont.m_waterMass += cont.m_waterMassDiff;

	cont.m_temperatureDiff = 0.0;
	cont.m_waterMassDiff = 0.0;

	if(cont.m_waterMass < 0) {
		std::cout << "Warning! Ground cont.m_waterMass: " << cont.m_waterMass << std::endl;
		retval = false;
	}

	return retval;
}

ClimateGenerator::ClimateGenerator(HeightMap& map, ClimateMap& climateMap, int32_t waterLevel) :
	m_waterLevel(waterLevel),m_map(map), m_climateMap(climateMap)
{
	InitializeClimate();
	m_stepCounter = 0;
	m_sunPosX = 0;
	m_sunPosY = m_map.GetSizeY() / 2;
	m_sunRadius = m_map.GetSizeY() / 2;
	m_sunStep = map.GetSizeX() / DAYLONG;
	
	m_timeStep = (DAYLONG*TIME_STEP)/(map.GetSizeX()/m_sunStep);
	//m_timeStep = (DAYLONG*TIME_STEP);
	
	m_sunVerticalMaximum = (MAX_SUN_LATITUDE*m_map.GetSizeY())/180;
	m_sunVerticalStepSize = (YEAR_LENGTH*DAYLONG*TIME_STEP)/(m_sunVerticalMaximum*4*m_timeStep);
	m_sunVerticalDirection = 1;
	m_sunVerticalStepCounter = 0;
	
	m_sunPosY -= m_sunVerticalMaximum;
	m_sunVerticalPos = -m_sunVerticalMaximum;
	
	
	//m_sunStep = 1;
	m_timeStepPerSquare = (DAYLONG*TIME_STEP)/(float)m_map.GetSizeX();
	m_coolingTemperature = 220;
}

ClimateGenerator::~ClimateGenerator()
{
}

static const int32_t CLIMATE_GENERATOR_SIG = 201803;
static const int32_t CLIMATE_GENERATOR_VERSION = 1;

bool ClimateGenerator::Load(std::istream& input)
{
	int32_t loadedVer = 0;
	int32_t loadedSign = 0;
	SerializableTG::Load( input, loadedSign);
	
	if(loadedSign != CLIMATE_GENERATOR_SIG) return false;
	
	SerializableTG::Load( input, loadedVer);
	
	if(loadedVer != CLIMATE_GENERATOR_VERSION) return false;
	
	SerializableTG::Load( input, m_waterLevel);
	SerializableTG::Load( input, m_sunPosX);
	SerializableTG::Load( input, m_sunPosY);
	SerializableTG::Load( input, m_sunRadius);
	SerializableTG::Load( input, m_sunStep);
	SerializableTG::Load( input, m_timeStep);
	SerializableTG::Load( input, m_timeStepPerSquare);
	SerializableTG::Load( input, m_coolingTemperature);
	
	InitializeClimate(false);
	
	return true;
		
}

bool ClimateGenerator::Store(std::ostream& output) const
{
	SerializableTG::Store(output , CLIMATE_GENERATOR_SIG);
	SerializableTG::Store(output , CLIMATE_GENERATOR_VERSION);
	
	SerializableTG::Store(output, m_waterLevel);
	SerializableTG::Store(output, m_sunPosX);
	SerializableTG::Store(output, m_sunPosY);
	SerializableTG::Store(output, m_sunRadius);
	SerializableTG::Store(output, m_sunStep);
	SerializableTG::Store(output, m_timeStep);
	SerializableTG::Store(output, m_timeStepPerSquare);
	SerializableTG::Store(output, m_coolingTemperature);
	
	return true;
}

void ClimateGenerator::InitializeClimate(bool newClimate)
{
	if(newClimate)
	{
		m_climateMap.Initialize(m_map.GetSizeX(), m_map.GetSizeY());
	}
	uint8_t height = 0;

	WaterContent* pWater = nullptr;
	AirContent* pAir = nullptr;
	GroundContent* pGround = nullptr;

	VOLUME_BASE = ((AIR_BASE_PRESSURE - ClimateUtils::CompAbsolutePressure(VOLUME_HEIGHT, AIR_BASE_PRESSURE))/GRAV_ACC)/(ClimateUtils::GetAirDensity(BASE_TEMPERATURE));

	for(int32_t y = 0; y < m_map.GetSizeY(); ++y) {
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x) {
			pWater = nullptr;
			pAir = nullptr;
			pGround = nullptr;
			
			ClimateCell& cell = m_climateMap.GetCellValue(x, y);
			if(newClimate)
			{
				height = m_map.GetCellValue(x, y);
				if(height < m_waterLevel) {
					// low water
					pWater = &cell.GetWaterContent();
					pWater->m_saltMass = (float) LOW_WATER_METERS * SALT_MASS_ON_SQUARE_METER ; // kg
					pWater->m_waterMass = (float) LOW_WATER_METERS * ClimateUtils::WATER_DENSITY; // kg of water
					pWater->m_temperature = BASE_TEMPERATURE;
					//pWater->m_surfaceLevel = LOW_WATER_METERS;
					cell.SetLowContent(CellContent::WATER);
					// low air
					pAir = &cell.GetAirContent();
					pAir->m_airMass = AIR_BASE_PRESSURE/GRAV_ACC;
					pAir->m_temperature = BASE_TEMPERATURE;
					//pAir->m_volumeBase = ((AIR_BASE_PRESSURE - ClimateUtils::CompAbsolutePressure(VOLUME_HEIGHT, AIR_BASE_PRESSURE))/GRAV_ACC)/(ClimateUtils::GetAirDensity(pAir->m_temperature));

				} else  {
					// low air
					pAir = &cell.GetAirContent();
					pAir->m_baseAltitude = (height - m_waterLevel) * GROUND_HEIGHT_TO_METERS;
					pAir->m_airMass = ClimateUtils::CompAbsolutePressure(pAir->m_baseAltitude, AIR_BASE_PRESSURE)/GRAV_ACC;
					pAir->m_temperature = ClimateUtils::CompRealAirTemp(BASE_TEMPERATURE, pAir->m_baseAltitude);
					//pAir->m_volumeBase = ((AIR_BASE_PRESSURE - ClimateUtils::CompAbsolutePressure(VOLUME_HEIGHT, AIR_BASE_PRESSURE))/GRAV_ACC)/(ClimateUtils::GetAirDensity(pAir->m_temperature));

					// ground
					pGround = &cell.GetGroundContent();
					//pGround->m_maxWaterCapacity = GROUND_WATER_METERS*ClimateUtils::WATER_DENSITY;
					pGround->m_temperature = pAir->m_temperature;//BASE_TEMPERATURE;
					cell.SetLowContent(CellContent::GROUND);
				}
			} else {
				// get old valid data
				pAir = &cell.GetAirContent();
				if(cell.IsCheckContent(CellContent::WATER))
				{
					pWater = &cell.GetWaterContent();
				} else {
					pGround = &cell.GetGroundContent();
				}
			}

			if(pAir) 
			{
				UpdateContent(*pAir);
			}
			
			if(pWater) 
			{
				UpdateContent(*pWater);
			}
			
			if(pGround) 
			{
				UpdateContent(*pGround);
			}
			
			
			int32_t nx, ny;
			for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) {
				m_climateMap.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), x, y, nx, ny);
				cell.SetNeightbour(dir, &m_climateMap.GetCellValue(nx, ny));
			}
		}

	}

}

void ClimateGenerator::SimulateClimateStep()
{
	//std::cout << "--------------START---------------" << std::endl;
	//writeClimateStatistics(std::cout, m_climateMap, m_waterLevel);
	SunHeatingStep();
	//SunHeatingDaylongStep();
	//std::cout << "--------------SUN---------------" << std::endl;
	//writeClimateStatistics(std::cout, m_climateMap, m_waterLevel);
	
	OneCellStep();
	
	//std::cout << "Added heat: " << m_oneStepHeat << " J" << std::endl;
	
	//float tempDiff = 0.1;
	//if(std::abs(m_oneStepHeat) > 100000000000)
	//{
	//	tempDiff *= 100;
	//}
	//else if(std::abs(m_oneStepHeat) > 1000000000)
	//{
	//	tempDiff *= 10;
	//}
	//
	//m_coolingTemperature += (m_oneStepHeat > 0)? -tempDiff : tempDiff;
	
	//std::cout << "--------------ONE CELL---------------" << std::endl;
	//writeClimateStatistics(std::cout, m_climateMap, m_waterLevel);

	AirMoveStep();
	//std::cout << "--------------AIR MOVE---------------" << std::endl;
	//writeClimateStatistics(std::cout, m_climateMap, m_waterLevel);

	WaterMoveStep();
	
	//std::cout << "--------------WATER MOVE---------------" << std::endl;
	//writeClimateStatistics(std::cout, m_climateMap, m_waterLevel);

	//AirMoveHighStep();
	//UpdateStep();
	//
	//AirMoveLowStep();

	UpdateStep();
	//std::cout << "--------------UPDATE---------------" << std::endl;
	//writeClimateStatistics(std::cout, m_climateMap, m_waterLevel);
	
	//std::cout << "--------------END---------------" << std::endl;

	//add sun step by one hour
	//m_sunPosX = (m_sunPosX + m_sunStep) % m_climateMap.GetSizeX();
	m_sunPosX = ((m_sunPosX - m_sunStep) > 0) ? (m_sunPosX - m_sunStep) : (m_sunPosX - m_sunStep) + m_climateMap.GetSizeX();
	
	m_sunVerticalStepCounter = (m_sunVerticalStepCounter + 1) % m_sunVerticalStepSize;
	if(m_sunVerticalStepCounter == 0)
	{
		m_sunPosY += m_sunVerticalDirection;
		m_sunVerticalPos += m_sunVerticalDirection;
		
		m_sunVerticalDirection = (std::abs(m_sunVerticalPos) < m_sunVerticalMaximum) ? m_sunVerticalDirection : -m_sunVerticalDirection;
	}	
	
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

	m_oneStepHeat = 0;

	// add warm to the surface
	for(int32_t y = m_sunPosY - (m_map.GetSizeY() / 2); y < m_sunPosY + (m_map.GetSizeY() / 2); ++y) {
		float degreeX = - (PI / 2);
		cosY = std::abs(std::cos(degreeY)) * REAL_SUN_POWER * m_timeStepPerSquare;
		for(int32_t x = m_sunPosX - (m_map.GetSizeY() / 2); x < m_sunPosX + (m_map.GetSizeY() / 2); ++x) {
			ClimateCell& cell = m_climateMap.GetCellValue(x,y);
			float degreeXtemp = degreeX;
			totalEnergy = 0;
			for (int32_t i = 0; (i < m_sunStep) && ((x + i) < (m_sunPosX  + (m_map.GetSizeY() / 2))); ++i) {
				totalEnergy += cosY * std::abs(std::cos(degreeXtemp)); // energy per time step
				degreeXtemp += degreeStep;
			}

			{
				AirContent &air = cell.GetAirContent();
				AddHeat(air, (totalEnergy * ATHMOSPHERIC_ABSORBTION));
				
				// immediate update
				air.m_temperature += air.m_temperatureDiff;
				air.m_temperatureDiff = 0;

				// is clouds
				if(air.m_cloudsHeight < MEASURE_HEIGHT) {
					//float ratio = clA*air.m_cloudsHeight + clB;
					//ratio = (ratio < 1) ? ratio : 1;
					totalEnergyGround -= (totalEnergy * CLOUDS_SCATTERED);
				}
			}
			m_oneStepHeat += totalEnergyGround;
			// air absortion
			totalEnergyGround -= (totalEnergy * ATHMOSPHERIC_ABSORBTION);

			//level = 1;
			if(cell.IsCheckContent(CellContent::GROUND)) {
				totalEnergyGround -= (totalEnergy*GROUND_REFLECTION);
				AddHeat(cell.GetGroundContent(), totalEnergyGround);
				cell.GetGroundContent().m_temperature += cell.GetGroundContent().m_temperatureDiff;
				cell.GetGroundContent().m_temperatureDiff = 0;
			} else {
				AddHeat(cell.GetWaterContent(), totalEnergyGround);
				cell.GetWaterContent().m_temperature += cell.GetWaterContent().m_temperatureDiff;
				cell.GetWaterContent().m_temperatureDiff = 0;
			}
			
			totalEnergyGround = totalEnergy;
			
			degreeX += degreeStep;
		}
		degreeY += degreeStep;
	}

	//std::cout << " Total Energy: " << m_oneStepHeat << std::endl;
	//m_oneStepHeat = -m_oneStepHeat / (float)(m_climateMap.GetSizeY()*m_climateMap.GetSizeX());
}

void ClimateGenerator::SunHeatingDaylongStep()
{
	static const float SIN_CONST = 114.59; // cumulated sinus multiplier
	static const float HEAT_PER_LINE = SIN_CONST * 0.5 * (((float)DAYLONG*TIME_STEP*REAL_SUN_POWER)/180); // cumulated sinus multiplier
	static const float degreeStep = PI / m_map.GetSizeY();
	//static const float clA = 1/(MEASURE_HEIGHT - CLOADS_UNDER_HEIGHT);
	//static const float clB = -CLOADS_UNDER_HEIGHT/(MEASURE_HEIGHT - CLOADS_UNDER_HEIGHT);

	float degreeY = - (PI / 2);
	
	float totalEnergy = 0;
	float totalEnergyGround = 0;

	m_oneStepHeat = 0;

	// add warm to the surface
	for(int32_t y = m_sunPosY - (m_map.GetSizeY() / 2); y < m_sunPosY + (m_map.GetSizeY() / 2); ++y) {
		totalEnergy = std::abs(std::cos(degreeY)) * HEAT_PER_LINE;
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x) {
			ClimateCell& cell = m_climateMap.GetCellValue(x,y);
			
			

			{
				AirContent &air = cell.GetAirContent();
				AddHeat(air, (totalEnergy * ATHMOSPHERIC_ABSORBTION));
				// update temperature
				air.m_temperature += air.m_temperatureDiff;
				air.m_temperatureDiff = 0;
				// is clouds
				if(air.m_cloudsHeight < MEASURE_HEIGHT) {
					//float ratio = clA*air.m_cloudsHeight + clB;
					//ratio = (ratio < 1) ? ratio : 1;
					totalEnergyGround -= (totalEnergy * CLOUDS_SCATTERED);
				}
			}
			m_oneStepHeat += totalEnergyGround;
			// air absortion
			totalEnergyGround -= (totalEnergy * ATHMOSPHERIC_ABSORBTION);

			//level = 1;
			if(cell.IsCheckContent(CellContent::GROUND)) {
				GroundContent &ground = cell.GetGroundContent();
				totalEnergyGround -= (totalEnergy*GROUND_REFLECTION);
				AddHeat(ground, totalEnergyGround);
				// update temperature
				ground.m_temperature += ground.m_temperatureDiff;
				ground.m_temperatureDiff = 0;
			} else {
				WaterContent &water = cell.GetWaterContent();
				// water
				AddHeat(water, totalEnergyGround);
				// update temperature
				water.m_temperature += water.m_temperatureDiff;
				water.m_temperatureDiff = 0;
			}
			
			totalEnergyGround = totalEnergy;
			
		}
		degreeY += degreeStep;
	}

	//std::cout << " Total Energy: " << m_oneStepHeat << std::endl;
	//m_oneStepHeat = -m_oneStepHeat / (float)(m_climateMap.GetSizeY()*m_climateMap.GetSizeX());
}

void ClimateGenerator::Cooling(ClimateCell& cell)
{
	const float AIR_HTC = HEAT_TRANSFER_COEFICIENT_AIR/ClimateUtils::GetSpecificHeat(CellContent::AIR);
	float resultTemp = 0.0;
	float energyLoss = 0.0;
	AirContent &air = cell.GetAirContent();
	//GroundContent *pGround = nullptr;
	//WaterContent *pLowWater = nullptr;
	//WaterContent *pDeepWater = nullptr;

	{
		//energyLoss = (ClimateUtils::GetThermalConductivity(CellContent::AIR) * ( m_coolingTemperature - air.m_temperature) * m_timeStep * 15000)/(15000 - air.m_baseAltitude); //
		float beginTemp = ClimateUtils::CompRealAirTemp(air.m_temperature, air.m_baseAltitude, 15000);
		// result temperature 
		resultTemp = ClimateUtils::CompCoolingTemperature(beginTemp, m_coolingTemperature, AIR_HTC/air.m_airMass, m_timeStep); //
		
		//temp = heat / (cont.m_airMass * ClimateUtils::GetSpecificHeat(CellContent::AIR));
		energyLoss = (beginTemp - resultTemp)*(air.m_airMass * ClimateUtils::GetSpecificHeat(CellContent::AIR));
		
		AddHeat(air, energyLoss);
		
		m_oneStepHeat += energyLoss;
		//AddHeat(air, m_oneStepHeat);
	}

	if(cell.IsCheckContent(CellContent::GROUND)) {
		GroundContent &ground = cell.GetGroundContent();
		energyLoss = ClimateUtils::GetThermalConductivity(CellContent::GROUND) * ( air.m_temperature - ground.m_temperature ) * m_timeStep;

		AddHeat(air, -energyLoss);
		AddHeat(ground, energyLoss);
		
		//energyLoss = (ClimateUtils::GetThermalConductivity(CellContent::GROUND) * (ground.m_temperature - ground.m_deepTemperature) * m_timeStep) / GROUND_HEAT_METERS;
		
		//AddHeat(ground, -energyLoss);
		//ground.m_deepTemperature += energyLoss / ((ClimateUtils::GROUND_DENSITY * (GROUND_HEAT_METERS - 1)) * ClimateUtils::GetSpecificHeat(CellContent::GROUND));
		
		
	}

	if(cell.IsCheckContent(CellContent::WATER)) {
		WaterContent &water = cell.GetWaterContent();
		energyLoss = ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( air.m_temperature - water.m_temperature ) * m_timeStep;

		AddHeat(air, -energyLoss);
		AddHeat(water, energyLoss);

		//energyLoss = (ClimateUtils::GetThermalConductivity(CellContent::WATER) * (water.m_temperature - water.m_deepTemperature) * m_timeStep) / LOW_WATER_METERS;
		
		//AddHeat(water, -energyLoss);
		//water.m_deepTemperature += energyLoss / (((water.m_waterMass + water.m_iceMass) - CompWaterMass(water)) * ClimateUtils::GetSpecificHeat(CellContent::WATER));
		
		//energyLoss = ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( water.m_temperature - DEEP_WATER_BASE_TEMPERATURE ) * m_timeStep;
		//AddHeat(water, -energyLoss);

	}

}

void ClimateGenerator::Precipitation(ClimateCell& cell)
{
	AirContent &air = cell.GetAirContent();


	if(air.m_cloudsHeight < RAIN_HEIGHT) {
		//float mass = CompAirMass(air, air.m_cloudsHeight);
		float mass = CompAirMass(air, air.m_baseAltitude + air.m_cloudsHeight);
		float waterMass = mass*air.m_actHum;
		float waterTemp = ClimateUtils::CompRealAirTemp(air.m_temperature, air.m_baseAltitude, air.m_cloudsHeight+air.m_baseAltitude);

		waterMass = (waterMass > air.m_waterMass) ? air.m_waterMass : waterMass;

		if(waterTemp < 0) {
			air.m_precip = CellContent::SNOW;
		} else {
			air.m_precip = CellContent::WATER;
		}

		air.m_waterMassDiff -= waterMass;

		if(cell.IsCheckContent(CellContent::WATER)) {
			WaterContent &water = cell.GetWaterContent();
			float wMass = CompWaterMass(water);
			float temp = ((waterMass*waterTemp) + (wMass*water.m_temperature))/(wMass + waterMass);
			float Q = waterMass*(waterTemp - temp)*ClimateUtils::GetSpecificHeat(CellContent::WATER);
			AddHeat(water, Q);
			water.m_evapWaterMass += waterMass;

		} else if(cell.IsCheckContent(CellContent::GROUND)) {
			GroundContent &ground = cell.GetGroundContent();
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

	for(int32_t y = 0; y < m_map.GetSizeY(); ++y) {
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x) {
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			// cooling
			Cooling(cell);

			Evaporation(cell);

			Precipitation(cell);

			//HeatSpread(cell);

			UpdateContent(cell.GetAirContent());
			if(cell.IsCheckContent(CellContent::WATER)) {
				UpdateContent(cell.GetWaterContent());
			} else if(cell.IsCheckContent(CellContent::GROUND)) {
				UpdateContent(cell.GetGroundContent());
			}

		}
	}

	//std::cout << "Joules added to planet: " << m_oneStepHeat << std::endl;
	//std::cout << "Cooling temperature: " << m_coolingTemperature << std::endl;

}

void ClimateGenerator::Evaporation(ClimateCell& cell)
{
	// bad - too much computing...:)

	AirContent &air = cell.GetAirContent();

	if(cell.IsCheckContent(CellContent::WATER)) {
		WaterContent &water = cell.GetWaterContent();
		
		water.m_evapWaterMass = 0;
		
		if(water.m_temperature < WATER_FREEZING) {
			return;
		}

		float maxHum = ClimateUtils::CompMaxAirHumidity((float)air.m_temperature, air.m_airPressure);

		if(maxHum < air.m_actHum) {
			return;
		}

		float lenght = air.m_lowWind.Length();
		//float waterMassEvap = (25.0 + 19.0*lenght)*(maxHum - air.m_actHum);
		float waterMassEvap = (25.0 + 9.0*lenght)*(maxHum - air.m_actHum);
		float heat = HEAT_OF_VAPORIZATION_OF_WATER * waterMassEvap;
		float heat2air = waterMassEvap*ClimateUtils::GetSpecificHeat(CellContent::AIR)*(water.m_temperature - air.m_temperature);

		//if(waterMassEvap < 0) {
		//	std::cout << "Evaporation failed..." << std::endl;
		//}

		water.m_evapWaterMass -= waterMassEvap;
		air.m_waterMassDiff += waterMassEvap;

		AddHeat(water, -heat);
		AddHeat(air, heat2air);
	} else if(cell.IsCheckContent(CellContent::GROUND)) {
		GroundContent &ground = cell.GetGroundContent();

		if((ground.m_temperature < WATER_FREEZING)||(ground.m_waterMass <= 0.0)) {
			return;
		}

		float maxHum = ClimateUtils::CompMaxAirHumidity((float)air.m_temperature, air.m_airPressure);

		if(maxHum < air.m_actHum) {
			return;
		}

		float lenght = air.m_lowWind.Length();
		//float waterMassEvap = (25.0 + 19.0*lenght)*(maxHum - air.m_actHum);
		float waterMassEvap = (25.0 + 9.0*lenght)*(maxHum - air.m_actHum);
		waterMassEvap = (ground.m_waterMass > waterMassEvap) ? waterMassEvap : ground.m_waterMass;

		float heat = HEAT_OF_VAPORIZATION_OF_WATER * waterMassEvap;
		float heat2air = waterMassEvap*ClimateUtils::GetSpecificHeat(CellContent::AIR)*(ground.m_temperature - air.m_temperature);

		if(waterMassEvap < 0) {
			std::cout << "Evaporation failed..." << std::endl;
		}

		ground.m_waterMassDiff -= waterMassEvap;
		air.m_waterMassDiff += waterMassEvap;

		AddHeat(ground, -heat);
		AddHeat(air, heat2air);

	}

}



void ClimateGenerator::AirForceComp(ClimateCell& cell, int32_t x, int32_t y, float sinLatitude)
{
	//static const float nS = SQUARE_SIDE_SIZE / 2; //neightbour surface
	//static const float nS = 0.5; //neightbour surface

	AirContent &air = cell.GetAirContent();

	//Urho3D::Vector2 topDiff;
	Urho3D::Vector2 lowDiff;


	air.m_maxPressureDiff = 0;
	air.m_maxVolumeDiff = 0;
	//air.m_lowForce = Urho3D::Vector2::ZERO;
	//air.m_highForce = Urho3D::Vector2::ZERO;

	// density in high * GRAV_ACC * nS
	float mass = CompDensity(air)*SQUARE_SIZE;


	Urho3D::Vector2 highDiff;
	float densHigh = CompDensityInAltitude(air, VOLUME_HEIGHT);
	float highConst = densHigh*GRAV_ACC*nS;
	float massHigh = densHigh*SQUARE_SIZE;

	float compPressureLow = air.m_airPressure - air.m_dynamicPressureLow;
	//float windSpeed = air.m_lowWind.Length();

	float dp;
	float lowForce;
	float highForce;
	
	//int32_t windDir = static_cast<int32_t>(GetWindDest(air.m_lowWind));
	//float altDiff = 0.0;

	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) {

		//m_climateMap.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), x, y, nx, ny);

		//AirContent &nAir = *m_climateMap.GetCellValue(nx,ny).GetAirContent(0);
		AirContent &nAir = cell.GetNeightbour(dir)->GetAirContent();

		{
			dp = compPressureLow - nAir.m_airPressure + nAir.m_dynamicPressureLow;
			lowForce = dp*nS;

			highForce = (air.m_volumeLevel > nAir.m_volumeLevel) ? (air.m_volumeLevel - nAir.m_volumeLevel)*highConst
			            : CompDensityInAltitude(nAir, VOLUME_HEIGHT)*GRAV_ACC*nS*(air.m_volumeLevel - nAir.m_volumeLevel);

			if(dp > air.m_maxPressureDiff) {
				air.m_maxPressureDiff = dp;
			}

			if((air.m_volumeLevel - nAir.m_volumeLevel) > air.m_maxVolumeDiff) {
				air.m_maxVolumeDiff = (air.m_volumeLevel - nAir.m_volumeLevel);
			}

			if(std::isinf(lowForce)||std::isnan(lowForce)) {
				std::cout << "Error! broken force: " << lowForce << " at " << x << ", " << y << std::endl;
			}

			Urho3D::Vector2 vec = DirVector[dir];

			//vec = vec*force;
			//lowForce /= m_timeStep;
			//highForce /= m_timeStep;

			lowDiff += vec*lowForce;
			highDiff += vec*highForce;
			
			//slowing down if wind is going uphill
			//altDiff = (dir == windDir) ? air.m_baseAltitude - nAir.m_baseAltitude : altDiff;

			// dynamic pressure comp == DRAG!
			lowDiff += nAir.m_dynamicForce - air.m_dynamicForce;
			highDiff += nAir.m_dynamicForceHigh - air.m_dynamicForceHigh;

		}

	}

	// force impuls length
	//float timeDiff = 0.1*std::sqrt(source.m_maxPressureDiff/dens*SQUARE_SIDE_SIZE);
	//timeDiff = (timeDiff > m_timeStep) ? m_timeStep : timeDiff;

	// mass from ground to MEASURE_HEIGHT
	//float massSource = source.m_airMass*(1 - ClimateUtils::CompAbsolutePressure(source.m_baseAltitude + MEASURE_HEIGHT, source.m_airPressure)/source.m_airPressure);

	// Coriolis effect
	//if(false) 
	{
		//Coriolis effect
		float f = 2*ANGULAR_VELOCITY*sinLatitude;

		Urho3D::Vector2 Fc = Urho3D::Vector2(air.m_lowWind.y_, -air.m_lowWind.x_) * f;
		lowDiff += Fc;

		Fc = Urho3D::Vector2(air.m_highWind.y_, -air.m_highWind.x_) * f;
		highDiff += Fc;
	}
	




	// wind speed change ver 1



	Urho3D::Vector2 windChange = lowDiff/mass;
	air.m_lowWind = air.m_lowWind + windChange;
	

	if(air.m_lowWind.Length() > WIND_SPEED_PROTECTION)
	{
		air.m_lowWind = air.m_lowWind.Normalized() * WIND_SPEED_PROTECTION;
	}
	

	windChange = highDiff/massHigh;
	air.m_highWind = air.m_highWind + windChange;

	if(air.m_highWind.Length() > HIGH_WIND_SPEED_PROTECTION)
	{
		air.m_highWind = air.m_highWind.Normalized() * HIGH_WIND_SPEED_PROTECTION;
	}


}


/*
void ClimateGenerator::AirLowWindMassChange(ClimateCell& sourceCell, int32_t x, int32_t y)
{
	static const float ratioT[2][2] = {{0.32325, 0.3535}, {0.17675,0.6465}};

	AirContent &source = sourceCell.GetAirContent();


	float massSource = (source.m_maxPressureDiff/GRAV_ACC)*0.5;

	// neightbour coords
	//int32_t nx, ny;
	// get receiver
	float speed = source.m_lowWind.Length();
	AirContent* pDest = nullptr;
	int32_t mainDir;
	float waterMass;

	if(massSource > (MASS_MOVE_PROTECTION*source.m_airMass)) {
		//std::cout << "Warning! critical mass move (low): " << massSource << " out of " << source.m_airMass << " at " << x << ", " << y << std::endl;
		massSource = MASS_MOVE_PROTECTION*source.m_airMass;
	}

	//if(std::isinf(speed)||std::isnan(speed))
	//{
	//	std::cout << "Error! speed fail (low): " << speed << " at " << x << ", " << y << std::endl;
	//	return;
	//}



	if(speed > 0) {
		// make it easier
		float volumeRatio = ((speed*m_timeStep*SQUARE_SIDE_SIZE)/SQUARE_SIZE);
		volumeRatio = (volumeRatio <= 1)? volumeRatio : 1;

		float massChange = volumeRatio*massSource;

		if(source.m_cloudsHeight < (MEASURE_HEIGHT)) {
			// all cloud is pushed away
			waterMass = (CompAirMass(source, source.m_cloudsHeight)*source.m_actHum)*volumeRatio;
		} else {
			// only humidity is moved
			waterMass = source.m_actHum*massChange; // water mass change per square meter
		}

		waterMass = (waterMass < (source.m_waterMass/2)) ? waterMass : (source.m_waterMass/2);

		if(source.m_airPressure > MIN_AIR_PRESSURE_PROTECTION) {
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

		for(int32_t i = 0; i < 3; ++i) {
			//m_climateMap.GetNeightbourCoords((MapContainerDIR)mainDir, nx, ny);
			//nx += x;
			//ny += y;
			//pDest = m_climateMap.GetCellValue(nx, ny).GetAirContent(0);
			pDest = &sourceCell.GetNeightbour(mainDir)->GetAirContent();

			maxPressProt = (pDest->m_airPressure > MAX_AIR_PRESSURE_PROTECTION);

			float r = ratio[(i%2)];

			if(!maxPressProt) {
				heatChange = massChange*r*ClimateUtils::GetSpecificHeat(CellContent::AIR)*
				             (ClimateUtils::CompRealAirTemp(source.m_temperature, source.m_baseAltitude, pDest->m_baseAltitude) - pDest->m_temperature);
			} else {
				heatChange = 0;
			}

			if(std::isnan(heatChange)) {
				std::cout << "AirWindMassChange Error" << std::endl;
			}

			AddHeat(*pDest, heatChange);

			if(!maxPressProt) {
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
 */ 

void ClimateGenerator::AirLowMassSpread(ClimateCell& sourceCell)
{
	static const float ratioT[2] = {1.0, 0.7071};

	AirContent &source = sourceCell.GetAirContent();

	// if pressure is too low skip air spread
	if(source.m_airPressure < MIN_AIR_PRESSURE_PROTECTION) {
		return;
	}

	//float dens = CompDensityInAltitude(source, VOLUME_HEIGHT);

	AirContent* pDest = nullptr;

	float windSpeed = source.m_lowWind.Length();

	float mass = (source.m_maxPressureDiff/GRAV_ACC) * PRESSURE_FACTOR;
	float massChunk = mass/(7.0 + windSpeed);


	float waterMass = (source.m_cloudsHeight < (MEASURE_HEIGHT))? 
					(CompAirMass(source, source.m_cloudsHeight)*source.m_actHum) :
					source.m_actHum * mass ;
	float waterMassChunk = waterMass/(7.0 + windSpeed);

	int32_t windDir = static_cast<int32_t>(GetWindDest(source.m_lowWind));

	float ratio;

	bool maxPressProt;

	float heatChange;

	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) {
		ratio = (windDir != dir)? ratioT[dir%2] : ratioT[dir%2]*windSpeed;

		//pDest = m_climateMap.GetCellValue(nx,ny).GetAirContent(0);
		pDest = &sourceCell.GetNeightbour(dir)->GetAirContent();

		maxPressProt = (pDest->m_airPressure > MAX_AIR_PRESSURE_PROTECTION);

		heatChange = (!maxPressProt)? massChunk*ratio*ClimateUtils::GetSpecificHeat(CellContent::AIR)*
		             (ClimateUtils::CompRealAirTemp(source.m_temperature, source.m_baseAltitude, pDest->m_baseAltitude) - pDest->m_temperature)
		             : 0 ;

		AddHeat(*pDest, heatChange);

		mass = massChunk*ratio;
		waterMass = waterMassChunk*ratio;

		if(!maxPressProt) {
			pDest->m_airMassDiff += mass;
		} else {
			// return to source
			source.m_airMassDiff += mass;
		}
		pDest->m_waterMassDiff += waterMass;

		source.m_airMassDiff -= mass;
		source.m_waterMassDiff -= waterMass;

	}

	//source.m_airMassDiff -= mass;
	//source.m_waterMassDiff -= waterMass;

}

/*
void ClimateGenerator::AirHighWindMassChange(ClimateCell& sourceCell, int32_t x, int32_t y)
{
	static const float ratioT[2][2] = {{0.32325, 0.3535}, {0.17675,0.6465}};

	AirContent& source = sourceCell.GetAirContent();

	float dens = CompDensityInAltitude(source, VOLUME_HEIGHT);

	// force impuls length - time to lost half of volume diffence for zero speed
	//float timeDiff = std::sqrt((2*source.m_maxVolumeDiff)/(SQUARE_SIDE_SIZE*GRAV_ACC));
	//timeDiff = (timeDiff > m_timeStep) ? m_timeStep : timeDiff;

	// mass from ground to MEASURE_HEIGHT
	//float massSource = source.m_airMass*(1 - ClimateUtils::CompAbsolutePressure(source.m_baseAltitude + MEASURE_HEIGHT, source.m_airPressure)/source.m_airPressure);

	float massSource = (source.m_maxVolumeDiff * dens)*0.5;


	// neightbour coords
	//int32_t nx, ny;
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
			//m_climateMap.GetNeightbourCoords((MapContainerDIR)mainDir, nx, ny);
			//nx += x;
			//ny += y;
			pDest = &sourceCell.GetNeightbour(mainDir)->GetAirContent();

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

/*
void ClimateGenerator::AirHighWindMassChange(ClimateCell& sourceCell, int32_t x, int32_t y)
{
	AirContent &source = sourceCell.GetAirContent();

	float dens = CompDensityInAltitude(source, source.m_baseAltitude + MEASURE_HEIGHT);



	//float massSource = (source.m_maxVolumeDiff * dens);

	// wind speed change
	//Urho3D::Vector2 windChange = (source.m_highForce*timeDiff)/(dens*SQUARE_SIZE);
	//source.m_highWind = source.m_highWind + windChange;

	// neightbour coords
	//int32_t nx, ny;
	// get receiver
	//float speed = source.m_highWind.Length();
	AirContent* destList[8];
	AirContent* pDest = nullptr;
	int32_t count = 0;

	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir)
	{

		//m_climateMap.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), x, y, nx, ny);

		//pDest = m_climateMap.GetCellValue(nx,ny).GetAirContent(0);
		pDest = &sourceCell.GetNeightbour(dir)->GetAirContent();

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
*/

void ClimateGenerator::AirHighMassSpread(ClimateCell& sourceCell)
{
	static const float ratioT[2] = {1.0, 0.7071};

	AirContent &source = sourceCell.GetAirContent();

	// if pressure is too low skip air spread
	if(source.m_airPressure < MIN_AIR_PRESSURE_PROTECTION) {
		return;
	}

	float dens = CompDensityInAltitude(source, VOLUME_HEIGHT);

	AirContent* pDest = nullptr;

	float windSpeed = source.m_highWind.Length();

	float mass = source.m_maxVolumeDiff * dens * VOLUME_FACTOR;
	float massChunk = mass/(7.0 + windSpeed);

	float waterMass = source.m_actHum * mass;
	float waterMassChunk = waterMass/(7.0 + windSpeed);

	int32_t windDir = static_cast<int32_t>(GetWindDest(source.m_highWind));

	float ratio;

	bool maxPressProt;

	float heatChange;

	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) {
		ratio = (windDir != dir)? ratioT[dir%2] : ratioT[dir%2]*windSpeed;

		//pDest = m_climateMap.GetCellValue(nx,ny).GetAirContent(0);
		pDest = &sourceCell.GetNeightbour(dir)->GetAirContent();

		maxPressProt = (pDest->m_airPressure > MAX_AIR_PRESSURE_PROTECTION);

		heatChange = (!maxPressProt)? massChunk*ratio*ClimateUtils::GetSpecificHeat(CellContent::AIR)*
		             (ClimateUtils::CompRealAirTemp(source.m_temperature, source.m_baseAltitude, pDest->m_baseAltitude) - pDest->m_temperature)
		             : 0 ;

		AddHeat(*pDest, heatChange);

		mass = massChunk*ratio;
		waterMass = waterMassChunk*ratio;

		if(!maxPressProt) {
			pDest->m_airMassDiff += mass;
		} else {
			// return to source
			source.m_airMassDiff += mass;
		}
		pDest->m_waterMassDiff += waterMass;

		source.m_airMassDiff -= mass;
		source.m_waterMassDiff -= waterMass;

	}

	//source.m_airMassDiff -= mass;
	//source.m_waterMassDiff -= waterMass;

}


MapContainerDIR ClimateGenerator::GetWindDest(Urho3D::Vector2& windVec)
{
	float angle = windVec.Angle(Urho3D::Vector2(0.0, -1.0));

	if(angle < 22.5) {
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
	//float tempAverage = 0.0;
	//int32_t count = 0;
	bool noError;
	for(int32_t y = 0; y < m_map.GetSizeY(); ++y) {
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x) {
			noError = true;
			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			AirContent &air = cell.GetAirContent();
			
			noError &= UpdateContent(air);
			
			//tempAverage += air.m_temperature;

			if(cell.IsCheckContent(CellContent::WATER)) {
				noError &= UpdateContent(cell.GetWaterContent());
			} else {
				noError &= UpdateContent(cell.GetGroundContent());
			}
			
			if(!noError)
			{
				std::cout << " Error cell: " << x << ", " << y << std::endl;
			}
			
		}
	}
	
	//tempAverage = tempAverage / ((float)m_map.GetTotalSize());
	
	//std::cout << "Temperature average: " << tempAverage << std::endl;
	
	//m_coolingTemperature += (tempAverage > BASE_TEMPERATURE) ? -0.5 : 0.5;
}

void ClimateGenerator::HeatSpread(ClimateCell& cell)
{
	ClimateCell *pN = nullptr;
	AirContent &air = cell.GetAirContent();
	float energyLoss;
	float temperature = cell.IsCheckContent(CellContent::WATER)?cell.GetWaterContent().m_temperature:cell.GetGroundContent().m_temperature;
	float tempIH;
	for(int32_t dir = 0; dir < 8; ++dir) {
		pN = cell.GetNeightbour(dir);
		AirContent &nAir = pN->GetAirContent();

		tempIH = ClimateUtils::CompRealAirTemp(nAir.m_temperature, nAir.m_baseAltitude, air.m_baseAltitude);

		if(air.m_temperature > tempIH) {

			energyLoss = (ClimateUtils::GetThermalConductivity(CellContent::AIR)
			              * m_timeStep
			              * ( air.m_temperature - tempIH )
			              * (VOLUME_HEIGHT - air.m_baseAltitude))
			             /(SQUARE_SIDE_SIZE);
			AddHeat(air, -energyLoss);
			AddHeat(nAir, energyLoss);
		}


		if(temperature > pN->GetContent(1)->m_temperature) {

			if(pN->IsCheckContent(CellContent::WATER)) {
				WaterContent &nWater = pN->GetWaterContent();
				energyLoss = (ClimateUtils::GetThermalConductivity(CellContent::WATER)
				              * m_timeStep
				              * ( temperature - nWater.m_temperature )
				              * LOW_WATER_METERS)
				             /(SQUARE_SIDE_SIZE);

				AddHeat(nWater, energyLoss);
			} else {
				GroundContent &nGround = pN->GetGroundContent();
				energyLoss = (ClimateUtils::GetThermalConductivity(CellContent::GROUND)
				              * m_timeStep
				              * ( temperature - nGround.m_temperature )
				              * GROUND_HEAT_METERS)
				             /(SQUARE_SIDE_SIZE);

				AddHeat(nGround, energyLoss);
			}

			if(cell.IsCheckContent(CellContent::WATER)) {
				AddHeat(cell.GetWaterContent(), -energyLoss);
			} else {
				AddHeat(cell.GetGroundContent(), -energyLoss);
			}
		}

	}
}

void ClimateGenerator::AirMoveStep()
{
	static const float degreeStep = PI / m_climateMap.GetSizeY();
	float degreeY = - (PI / 2);
	float sinLatitude;

	for(int32_t y = 0; y < m_map.GetSizeY(); ++y) {
		sinLatitude = std::sin(degreeY);
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x) {

			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			//AirContent &air = *cell.GetAirContent(0);

			AirForceComp(cell, x,y, sinLatitude);

			AirHighMassSpread(cell);
			//AirHighWindMassChange(cell, x, y);
			//AirHighWindMassChange2(air, x, y);
			
			AirLowMassSpread(cell);
			//AirLowWindMassChange(cell, x, y);

		}

		degreeY += degreeStep;
	}
}

/*
void ClimateGenerator::WaterForceComp(ClimateCell& waterCell, int32_t x, int32_t y, float sinLatitude)
{
	//static const float nS = SQUARE_SIDE_SIZE / 2; //neightbour surface



	WaterContent &water = waterCell.GetWaterContent();

	//int32_t nx, ny;

	Urho3D::Vector2 forceDiff(Urho3D::Vector2::ZERO);
	water.m_maxSurfaceDiff = 0;

	float force;
	float c = ClimateUtils::WATER_DENSITY*GRAV_ACC*nS;

	if(water.m_waterMass <= 0) {
		// no move anymore
		water.m_streamDir = Urho3D::Vector2::ZERO;
		return;
	}

	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) {

		//m_climateMap.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), x, y, nx, ny);

		//ClimateCell &nCell = m_climateMap.GetCellValue(nx,ny);
		ClimateCell &nCell = *waterCell.GetNeightbour(dir);

		if(nCell.IsCheckContent(CellContent::WATER)) {
			WaterContent &wc = nCell.GetWaterContent();

			force = (water.m_surfaceLevel - wc.m_surfaceLevel)*c;

			if(water.m_maxSurfaceDiff < (water.m_surfaceLevel - wc.m_surfaceLevel)) {
				water.m_maxSurfaceDiff = (water.m_surfaceLevel - wc.m_surfaceLevel);
			}

			if(std::isinf(force)||std::isnan(force)) {
				std::cout << "Error! broken force: " << force << " at " << x << ", " << y << std::endl;
			}

			Urho3D::Vector2 vec = DirVector[dir];

			//vec = vec*force;
			forceDiff += vec*force;

			// dynamic pressure comp
			//forceDiff += wc.m_streamDir.Normalized()*wc.m_dynamicPressure*nS;
			forceDiff += wc.m_dynamicForce - water.m_dynamicForce;

		} else {
			forceDiff -= water.m_dynamicForce;
		}

	}

	// Coriolis effect
	//if(true)
	{
		//Coriolis effect
		float f = 2*ANGULAR_VELOCITY*sinLatitude;

		Urho3D::Vector2 Fc = Urho3D::Vector2(water.m_streamDir.y_, -water.m_streamDir.x_) * f;
		forceDiff += Fc;

	}

	Urho3D::Vector2 streamChange = forceDiff/(ClimateUtils::WATER_DENSITY*SQUARE_SIZE);

	// 0.2 means 2*(1/g) where g == 10 ver 2
	//Urho3D::Vector2 windChange = lowDiff.Normalized()*((air.m_maxPressureDiff*0.1)/(m_timeStep)) ;

	// ver 3
	//Urho3D::Vector2 windChange = lowDiff.Normalized()*0.5 ;

	//float acc = windChange.Length();
	//windChange = windChange*m_timeStep;

	//if(windChange.Length() > WIND_SPEED_CHANGE_PROTECTION)
	//{
	//	windChange = windChange.Normalized() * WIND_SPEED_CHANGE_PROTECTION;
	//}

	water.m_streamDir += streamChange;

}
 */
 
/*
void ClimateGenerator::WaterStreamMassChange(ClimateCell& sourceCell, int32_t x, int32_t y)
{
	static const float ratioT[2][2] = {{0.32325, 0.3535}, {0.17675,0.6465}};

	WaterContent &source = sourceCell.GetWaterContent();

	float massSource = (source.m_maxSurfaceDiff * ClimateUtils::WATER_DENSITY)*0.5;


	// get receiver
	float speed = source.m_streamDir.Length();
	WaterContent* pDest = nullptr;
	int32_t mainDir;

	if(source.m_waterMass <= 0) {
		//no move anymore
		source.m_streamDir = Urho3D::Vector2::ZERO;
		return;
	}

	// simulation protection
	if(massSource > (MASS_MOVE_PROTECTION*source.m_waterMass)) {
		//std::cout << "Warning! critical mass move (high): " << massSource << " out of " << source.m_airMass << " at " << x << ", " << y << std::endl;
		massSource = MASS_MOVE_PROTECTION*source.m_waterMass;
	}

	if(speed > 0) {
		// make it easier
		float volumeRatio = ((speed*m_timeStep*SQUARE_SIDE_SIZE)/SQUARE_SIZE);
		volumeRatio = (volumeRatio <= 1)? volumeRatio : 1;

		float massChange = volumeRatio*massSource;

		source.m_waterMassDiff -= massChange;

		// get destinations
		mainDir = (int32_t)GetWindDest(source.m_streamDir);
		mainDir = (mainDir + 7) % 8;
		const float *ratio = ratioT[(mainDir % 2)];

		for(int32_t i = 0; i < 3; ++i) {
			float r = ratio[(i%2)];

			if(sourceCell.GetNeightbour(mainDir)->IsCheckContent(CellContent::WATER)) {
				pDest = &sourceCell.GetNeightbour(mainDir)->GetWaterContent();


				float heatChange = massChange*r*ClimateUtils::GetSpecificHeat(CellContent::WATER)*(source.m_temperature - pDest->m_temperature);

				if(std::isnan(heatChange)) {
					std::cout << "water heatChange Error" << std::endl;
				}

				AddHeat(*pDest, heatChange);

				pDest->m_waterMassDiff += massChange*r;

			} else {
				source.m_waterMassDiff += massChange*r;
			}

			mainDir = (mainDir + 1) % 8;
		}
	}
}
*/


void ClimateGenerator::WaterMassSpread(ClimateCell& sourceCell)
{
	static const float ratioT[2] = {1.0, 0.7071};

	WaterContent &source = sourceCell.GetWaterContent();
	
	float mass;
	
	// waterMass protection
	
	WaterContent* pDest = nullptr;

	float heatChange;
	
	float ratio;

	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
	{
		
		if(!sourceCell.GetNeightbour(dir)->IsCheckContent(CellContent::WATER)) continue;
		
		ratio = ratioT[dir%2];

		//pDest = m_climateMap.GetCellValue(nx,ny).GetAirContent(0);
		pDest = &sourceCell.GetNeightbour(dir)->GetWaterContent();
		
		mass = source.m_evapWaterMass - pDest->m_evapWaterMass;
		
		if(mass < 0) continue;
		
		heatChange = ratio*(ClimateUtils::GetThermalConductivity(CellContent::WATER) * ( source.m_temperature - pDest->m_temperature ) * m_timeStep * LOW_WATER_METERS)/SQUARE_SIDE_SIZE;
		//heatChange = mass*ratio*ClimateUtils::GetSpecificHeat(CellContent::WATER)*(source.m_temperature - pDest->m_temperature);

		AddHeat(*pDest, heatChange);
		AddHeat(source, -heatChange);

	}

	//source.m_airMassDiff -= mass;
	//source.m_waterMassDiff -= waterMass;

}


void ClimateGenerator::WaterMoveStep()
{
	//static const float degreeStep = PI / m_climateMap.GetSizeY();
	//float degreeY = - (PI / 2);
	//float sinLatitude;

	for(int32_t y = 0; y < m_map.GetSizeY(); ++y) {
		//sinLatitude = std::sin(degreeY);
		for(int32_t x = 0; x < m_map.GetSizeX(); ++x) {

			ClimateCell &cell = m_climateMap.GetCellValue(x,y);
			if(cell.IsCheckContent(1, CellContent::WATER)) 
			{
			//	//WaterContent &water = *cell.GetWaterContent(1);
			//
			//	WaterForceComp(cell, x,y, sinLatitude);
			//
				WaterMassSpread(cell);
			//	//WaterStreamMassChange(cell, x, y);
			//
			} else {
				// move water around ground 
				GroundWaterFlow(cell);
			}

		}

		//degreeY += degreeStep;
	}
}

void ClimateGenerator::GroundWaterFlow(ClimateCell& sourceCell)
{
	static const float ratioT[2] = {1.0, 0.7071};
	
	AirContent &air = sourceCell.GetAirContent();
	GroundContent &ground = sourceCell.GetGroundContent();
	
	ground.m_waterFlow = 0.0; // last cycle is fergoten
	
	// if moisture is snow, no water moving
	if((air.m_temperature < WATER_FREEZING)||(ground.m_waterMass < GROUND_WATER_CAPACITY))
	{
		// reset counter
		ground.m_waterFlowCont = 0;
		return;
	}
	
	// lakes can overflow
	float baseAltitude = air.m_baseAltitude + (ground.m_waterMass/ClimateUtils::WATER_DENSITY);
	
	ClimateCell *pDest;
	
	float waterMassChunk = (GROUND_WATER_RUNOFF * (ground.m_waterMass - GROUND_WATER_CAPACITY)) / 7.0;
	
	float waterMass;
	
	float heatChange;
	
	float ratio;

	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
	{
		ratio = ratioT[dir%2];
		waterMass = waterMassChunk*ratio;
		
		pDest = sourceCell.GetNeightbour(dir);
		
		if(pDest->IsCheckContent(CellContent::WATER))
		{
			heatChange = waterMass*ClimateUtils::GetSpecificHeat(CellContent::WATER)*(ground.m_temperature - pDest->GetWaterContent().m_temperature);
			ground.m_waterMassDiff -= waterMass;
			ground.m_waterFlow += waterMass;
			AddHeat(pDest->GetWaterContent(), heatChange);
		} else {
			// ground
			// water flows down
			if(baseAltitude > pDest->GetAirContent().m_baseAltitude)
			{
				heatChange = waterMass*ClimateUtils::GetSpecificHeat(CellContent::WATER)*(ground.m_temperature - pDest->GetGroundContent().m_temperature);
				ground.m_waterMassDiff -= waterMass;
				ground.m_waterFlow += waterMass;
				pDest->GetGroundContent().m_waterMassDiff += waterMass;
				AddHeat(pDest->GetGroundContent(), heatChange);
			}
		}

	}
	
	ground.m_waterFlowCont = (ground.m_waterFlow > 0)? (ground.m_waterFlowCont + 1) : 0;
	
}


