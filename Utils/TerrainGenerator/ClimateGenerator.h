#ifndef __CLIMATE_GENERATOR__H__
#define __CLIMATE_GENERATOR__H__

#include "MapContainer.h"
#include "ClimateCell.h"

#include <string>

class ClimateGenerator
{
public:
	class ClimateException : public std::exception {
		std::string m_errStr;
	public:
		ClimateException(const std::string& errStr) : m_errStr(errStr) {}
		
		virtual const char* what() const _GLIBCXX_USE_NOEXCEPT
		{
			return m_errStr.c_str();
		}
	};
private:
	float m_oneStepHeat;
	float m_timeStep;
	int32_t m_sunPosX; // Sun positionX
	int32_t m_sunPosY; // Sun positionY
	int32_t m_sunRadius;
	int32_t m_sunStep;
	int32_t m_waterLevel;
	MapContainer<uint8_t, SphereMapCoords>& m_map;
	MapContainer<ClimateCell, SphereMapCoords>& m_climateMap;
public:
	ClimateGenerator(MapContainer<uint8_t, SphereMapCoords>& map, MapContainer<ClimateCell, SphereMapCoords>& climateMap, int32_t waterLevel);
	~ClimateGenerator();
	
	void InitializeClimate();
	
	void SimulateClimateStep();
	
	void GetSunPosition(int32_t &x, int32_t &y) { x = m_sunPosX; y = m_sunPosY; }

public:
	// help functions
	static float CompCloudsHeight(AirContent& cont);

private:
		
	void AddTempToIncMap(int32_t x, int32_t y, int32_t level, float deltaTemp);
	void AddHeatIncToClimate();
	
	MapContainerDIR GetWindDest(Urho3D::Vector2 &windVec);
	
	void Evaporation(ClimateCell& cell);
	void AirForceComp(ClimateCell &air, int32_t x, int32_t y, float sinLatitude);
	void AirLowWindMassChange(ClimateCell &source, int32_t x, int32_t y);
	void AirHighWindMassChange(ClimateCell &source, int32_t x, int32_t y);
	
	void WaterForceComp(ClimateCell &water, int32_t x, int32_t y, float sinLatitude);
	void WaterStreamMassChange(ClimateCell &source, int32_t x, int32_t y);
	
	void Cooling(ClimateCell &cell);
	void Precipitation(ClimateCell &cell);
	
	void SunHeatingStep();
	void OneCellStep();
	void AirMoveStep();
	void WaterMoveStep();
	void UpdateStep();
	
	float GetSalinity(WaterContent &water);
	
};

#endif // __CLIMATE_GENERATOR__H__
