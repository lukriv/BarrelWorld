#ifndef __CLIMATE_GENERATOR__H__
#define __CLIMATE_GENERATOR__H__

#include "MapContainer.h"
#include "ClimateCell.h"

class ClimateGenerator
{
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

public:
	// help functions
	static float CompCloudsHeight(AirContent& cont);

private:
		
	void AddTempToIncMap(int32_t x, int32_t y, int32_t level, float deltaTemp);
	void AddHeatIncToClimate();
	
	MapContainerDIR GetWindDest(Urho3D::Vector2 &windVec);
	
	void AirPressureChange(AirContent & air);
	void Evaporation(AirContent &air, WaterContent &water);
	void AirMoveChange(AirContent &air, int32_t x, int32_t y);
	void AirWind(AirContent &air, int32_t x, int32_t y);
	
	
	void SunHeatingStep();
	void CoolingStep();
	void AirPressureDiffStep();
	void AirMoveStep();
	
	float GetSalinity(WaterContent &water);
	
};

#endif // __CLIMATE_GENERATOR__H__
