#ifndef __CLIMATE_GENERATOR__H__
#define __CLIMATE_GENERATOR__H__

#include "HeightMap.h"
#include "ClimateMap.h"
#include "SerializableTG.h"

#include <string>

class ClimateGeneratorHex : public SerializableTG
{
public:
	class ClimateException : public std::exception
	{
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
	float m_timeStepPerSquare;
	float m_coolingTemperature;
	int32_t m_sunPosX; // Sun positionX
	int32_t m_sunPosY; // Sun positionY
	int32_t m_sunRadius;
	int32_t m_sunStep;
	int32_t m_sunStepSize;
	
	int32_t m_sunVerticalPos;
	int32_t m_sunVerticalStepSize;
	int32_t m_sunVerticalDirection;
	int32_t m_sunVerticalMaximum;
	int32_t m_sunVerticalStepCounter;
	
	int32_t m_waterLevel;
	
	int32_t m_stepCounter;

	HeightMap& m_map;
	ClimateMap& m_climateMap;
public:
	ClimateGenerator(HeightMap& map, ClimateMap& climateMap, int32_t waterLevel);
	~ClimateGenerator();

	void SimulateClimateStep();

	void GetSunPosition(int32_t &x, int32_t &y)
	{
		x = m_sunPosX;
		y = m_sunPosY;
	}
	
	int32_t GetWaterLevel() { return m_waterLevel; }
	float GetCoolingTemp() { return m_coolingTemperature; }

	virtual	bool Store(std::ostream& output) const override;
	virtual	bool Load(std::istream& input) override;

public:
	// help functions
	static float CompCloudsHeight(AirContent& cont);

private:
	
	void InitializeClimate(bool newClimate = true);
	
	//void AddTempToIncMap(int32_t x, int32_t y, int32_t level, float deltaTemp);
	//void AddHeatIncToClimate();

	MapContainerDIR GetWindDest(Urho3D::Vector2 &windVec);

	void Evaporation(ClimateCell& cell);
	void Cooling(ClimateCell &cell);
	void Precipitation(ClimateCell &cell);
	void HeatSpread(ClimateCell& cell);


	void AirForceComp(ClimateCell &air, int32_t x, int32_t y, float sinLatitude);
	//void AirLowWindMassChange(ClimateCell &source, int32_t x, int32_t y);
	//void AirHighWindMassChange(ClimateCell &source, int32_t x, int32_t y);
	void AirLowMassSpread(ClimateCell& sourceCell);
	void AirHighMassSpread(ClimateCell& sourceCell);

	//void WaterForceComp(ClimateCell &water, int32_t x, int32_t y, float sinLatitude);
	void WaterMassSpread(ClimateCell& sourceCell);
	//void WaterStreamMassChange(ClimateCell &source, int32_t x, int32_t y);
	
	void GroundWaterFlow(ClimateCell& sourceCell);


	void SunHeatingStep();
	void SunHeatingDaylongStep();
	void OneCellStep();
	void AirMoveStep();
	void WaterMoveStep();
	void UpdateStep();

	float GetSalinity(WaterContent &water);

	
};

#endif // __CLIMATE_GENERATOR__H__
