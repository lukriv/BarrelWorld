#ifndef __CLIMATE_GENERATOR_UTILS__H__
#define __CLIMATE_GENERATOR_UTILS__H__

#include "ClimateCell.h"



class ClimateUtils
{

public:
	static const float WATER_DENSITY;
	static const float AIR_DENSITY;
	static const float ICE_DENSITY;
	static const float GROUND_DENSITY;
	
	static const float STEFAN_BOLTZMAN_CONSTANT;
public:
	static float GetDewPointTemperature(float relHum);
	
	// temperature in Celsius
	// salinity in g/liter or kg/m^3
	inline static float GetWaterDensity(int32_t temperature, float salinity)
	{
		if(temperature < 273) temperature = 273;
		if(temperature > 373) temperature = 373;
		
		temperature = temperature - 273;
		
		float koef = 0.8*salinity + 998.52;
		return (-0.3663*(float)(temperature*temperature) + 1.6315*(float)(temperature) + koef);
	}
	
	inline static float GetAirDensity(int32_t temperature)
	{
		if(temperature < CLIMATE_MIN_TEMPERATURE_KELVIN) temperature = CLIMATE_MIN_TEMPERATURE_KELVIN;
		if(temperature > CLIMATE_MAX_TEMPERATURE_KELVIN) temperature = CLIMATE_MAX_TEMPERATURE_KELVIN;
		
		temperature = temperature - 273;
		
		return (-0.0047 * (float)(temperature) + 1.32);
	}
	
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
	
	// pressure in altitude delta
	inline static float GetPressureInHeight2( float deltaAltitude, float basePressure, float temperature)
	{
		// p = p0*e^(- (Density0 * g * deltaAltitude) / p0)
		return basePressure * std::exp(- (GetAirDensity(temperature) * 10 * deltaAltitude)/basePressure);
	}
	
	// pressure in altitude delta
	inline static float GetPressureInHeight2( float deltaAltitude, float basePressure)
	{
		// p = p0*e^(- (Density0 * g * deltaAltitude) / p0)
		return basePressure * std::exp(- (AIR_DENSITY * 10 * deltaAltitude)/basePressure);
	}
	
	// compute relative pressure from absolute pressure in altitude
	inline static float CompRelativePressure( float altitude, float absPressure)
	{
		// p = p0*e^(- (Density0 * g * deltaAltitude) / p0)
		return absPressure / std::pow((1 - altitude / 44330),5.255);
	}
	
	// compute absolute pressure in altitude from relative pressure
	inline static float CompAbsolutePressure( float altitude, float relPressure)
	{
		// p = p0*e^(- (Density0 * g * deltaAltitude) / p0)
		return relPressure * std::pow((1 - altitude / 44330),5.255);
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

	inline static float CompCoolingTemperature(float baseTemperature, float envTemperature, float coef, float timeChunk)
	{
		return envTemperature + (baseTemperature - envTemperature)*std::exp(-coef*timeChunk);
	}
	
	inline static float CompRadiationPerSquareMeter(float temperature)
	{
		return (STEFAN_BOLTZMAN_CONSTANT*temperature*temperature*temperature*temperature); // M = sigma*T^4 
	}
	
};

#endif // __CLIMATE_GENERATOR_UTILS__H__
