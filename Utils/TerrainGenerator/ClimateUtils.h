#ifndef __CLIMATE_GENERATOR_UTILS__H__
#define __CLIMATE_GENERATOR_UTILS__H__

#include "ClimateCell.h"

class ClimateUtils
{

public:
	static float Initialize();

	static float GetMaxAirHumidity(int32_t temperature);
	
	// temperature in Celsius
	// salinity in g/liter or kg/m^3
	static float GetWaterDensity(int32_t temperature, float salinity);

	static float GetAirDensity(int32_t temperature);
	
	static float GetGroundDensity();
	
	// J/(kg*K)
	static float GetSpecificHeat(CellContent::ContentType contType);
	
	// W/(m*K)
	static float GetThermalConductivity(CellContent::ContentType contType);
	
	// Compute real temperature at given altitude
	// baseTemperature - temperature in altitude 0
	// altitude (in meters)
	inline static float CompRealAirTemp(float baseTemperature, float altitude)
	{
		return (baseTemperature - 0.005*altitude);
	}
	
	// Compute real temperature at given altitude
	// baseTemperature - temperature at baseAltitude
	// baseAltitude (in meters)
	// altitude (in meters)
	inline static float CompRealAirTemp(float baseTemperature, float baseAltitude, float altitude)
	{
		return (baseTemperature - 0.005*(altitude - baseAltitude));
	}
	
	// pressure in height x
	inline static float GetPressureInHeight( float altitude, float basePressure)
	{
		// (1 / 11000) == 0.00009091
		return basePressure * (1 - 0.00009091 * altitude );
	}
	
};

#endif // __CLIMATE_GENERATOR_UTILS__H__
