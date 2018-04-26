#ifndef __CLIMATE_CELL__H__
#define __CLIMATE_CELL__H__

#include <Urho3D/Math/Vector2.h>
#include <cstdint>


const float CLIMATE_MIN_TEMPERATURE = -50;
const float CLIMATE_MIN_TEMPERATURE_KELVIN = -50 + 273;
const float CLIMATE_MAX_TEMPERATURE = 150;
const float CLIMATE_MAX_TEMPERATURE_KELVIN = 150 + 273;

class CellContent {
public:
	enum ContentType {
		NONE,
		GROUND,
		AIR,
		WATER,
		ICE
	};
private:
	ContentType m_type;
public:
	float m_temperature; // temperature [K]
public:
	CellContent(ContentType type) : m_type(type), m_temperature(15) {}
	ContentType GetContentType() const { return m_type; }
};

class WaterContent : public CellContent {
public:
	float m_saltMass; 
	float m_waterMass;
	float m_iceMass;
	Urho3D::Vector2 m_streamDir; // vector of water stream
	
	WaterContent() : CellContent(CellContent::WATER)
		, m_saltMass(0) 
		, m_waterMass(0)
		, m_iceMass(0) 
	{}
	
};

class AirContent : public CellContent {
public:
	float m_baseAltitude;
	float m_airPressure; // air pressure at altitude 0 [Pa]
	float m_waterMass; // kilogram of water as vapour [kg]
	
	// updatable
	float m_airMass; // air mass (can be computed from pressure ) [kg]
	float m_actHum; // actual air humidity [kg/kg]
	
	// helper
	float m_temperatureDiff; // temperature difference from the last step
	float m_airPressureDiff; // 
	float m_waterMassDiff;
	
	Urho3D::Vector2 m_highForce; // hor. dir. of wind in heght
	Urho3D::Vector2 m_lowForce; // horizontal direction of wind at ground level
	
	Urho3D::Vector2 m_highWind; // hor. dir. of wind in heght
	Urho3D::Vector2 m_lowWind; // horizontal direction of wind at ground level
	
	AirContent() : CellContent(CellContent::AIR)
		, m_baseAltitude(0)
		, m_airPressure(100000.0) // air pressure
		, m_waterMass(0)
		, m_airMass(0)
		, m_actHum(0)
		, m_temperatureDiff(0)
		, m_airPressureDiff(0.0)
		, m_waterMassDiff(0)
	{}
};

class GroundContent : public CellContent {
public:
	float m_maxWaterCapacity;
	float m_waterMass; // mass of water on the ground
	
	GroundContent() : CellContent(CellContent::GROUND)
		, m_maxWaterCapacity(0)
		, m_waterMass(0)
	{}
};

#define CLIMATE_LEVELS 3

class ClimateCell
{
	
	CellContent *m_levels[CLIMATE_LEVELS];
public:
	ClimateCell();
	~ClimateCell();
	
	CellContent* GetContent(int32_t level);
	
	CellContent* GetContent(int32_t level) const;
	
	bool IsCheckContent(int32_t level, CellContent::ContentType type) const;
	
	AirContent* GetAirContent(int32_t level);
	GroundContent* GetGroundContent(int32_t level);
	WaterContent* GetWaterContent(int32_t level);
	
	void AddContent( int32_t level, CellContent* pContent );
	
};

#endif // __CLIMATE_CELL__H__
