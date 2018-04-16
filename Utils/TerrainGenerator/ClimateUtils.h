#ifndef __CLIMATE_GENERATOR_UTILS__H__
#define __CLIMATE_GENERATOR_UTILS__H__

#include "ClimateCell.h"

class ClimateUtils
{

public:
	static float Initialize();

	static float GetMaxAirHumidity(int32_t temperature);
	
	static float GetDewPointTemperature(float relHum);
	
	// temperature in Celsius
	// salinity in g/liter or kg/m^3
	static float GetWaterDensity(int32_t temperature, float salinity);

	static float GetAirDensity(int32_t temperature);
	
	static float GetGroundDensity();
	
	// J/(kg*K)
	static float GetSpecificHeat(CellContent::ContentType contType);
	
	// W/(m*K)
	static float GetThermalConductivity(CellContent::ContentType contType);
	
	// pressure in height x
	inline static float GetPressureInHeight( float altitude, float basePressure)
	{
		// (1 / 11000) == 0.00009091
		return basePressure * (1 - 0.00009091 * altitude );
	}
	
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
	
	inline static float CompAltFromTemp(float baseTemperature, float baseAltitude, float temperature)
	{
		return (200*(baseTemperature - temperature) - baseAltitude);
	}
	
	
	inline static float CompMaxAirHumidity(float temperature, float pressure)
	{
		float vapPressMax;
		if(temperature < 273)
		{
			vapPressMax = std::exp(28.926 - (6148.0/temperature));
		} else {
			vapPressMax = std::exp(23.58 - (4044.2/(temperature - 38.5)));
		}
		
		return 0.622*(vapPressMax/(pressure - vapPressMax));
	}
	
	inline static float CompDewPointTemperature(float airHumidity, float pressure)
	{
		float helper = std::log((airHumidity*pressure)/(0.622 + airHumidity));
		float temp;
		if(airHumidity < 0.004)
		{
			temp = 6148.0/(28.926 - helper);
		} else {
			temp = (4044.2/(23.58 - helper)) + 38.5;
		}
		
		return temp;
	}

	
};

#endif // __CLIMATE_GENERATOR_UTILS__H__
