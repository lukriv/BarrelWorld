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
		ICE,
		SNOW
	};
private:
	ContentType m_type;
public:
	float m_temperature; // temperature [K]
	
	// helper
	float m_temperatureDiff; // temperature difference from the last step
public:
	CellContent(ContentType type) : m_type(type)
		, m_temperature(15)
		, m_temperatureDiff(0) {}
	ContentType GetContentType() const { return m_type; }
};

class WaterContent : public CellContent {
public:
	float m_saltMass; 
	float m_waterMass;
	float m_iceMass;
	
	// updatable 
	float m_dynamicPressure; // dynamic pressure ( from Bernoulli's principle )
	float m_surfaceLevel; // for water surface level difference
	
	// helper
	float m_waterMassDiff;
	float m_iceMassDiff;
	
	float m_maxSurfaceDiff;
	
	Urho3D::Vector2 m_force; // force to stream move
	
	Urho3D::Vector2 m_streamDir; // vector of water stream
	
	WaterContent() : CellContent(CellContent::WATER)
		, m_saltMass(0) 
		, m_waterMass(0)
		, m_iceMass(0) 
		, m_dynamicPressure(0)
		, m_surfaceLevel(0)
		, m_waterMassDiff(0)
		, m_iceMassDiff(0)
	{}
	
};

class AirContent : public CellContent {
public:
	float m_baseAltitude;
	float m_airMass; // air mass [kg]
	float m_waterMass; // kilogram of water as vapour [kg]
	float m_volumeLevel; // volume level at measure level
	
	// updatable
	float m_airPressure; // air pressure at altitude 0 (can be computed from air mass ) [Pa]
	float m_dynamicPressureLow; // dynamic pressure ( from Bernoulli's principle ) 
	//float m_dynamicPressureHigh; // dynamic pressure ( from Bernoulli's principle ) 
	float m_actHum; // actual air humidity [kg/kg]
	float m_cloudsHeight; // altitude of clouds
	CellContent::ContentType m_precip; // precipitation type
	
	// helper
	float m_volumeBase; // initial base volume constant
	float m_maxPressureDiff; // maximal pressure difference between neightbour cells
	float m_maxVolumeDiff; // maximal volume difference between neightbour cells
	
	
	float m_airMassDiff; // air mass difference
	float m_waterMassDiff;
	
	Urho3D::Vector2 m_dynamicForce;
	
	//Urho3D::Vector2 m_lowForce; // force direction between neightbour cells
	
	//Urho3D::Vector2 m_highForce; // high force direction between neightbour cells

	Urho3D::Vector2 m_lowWind; // horizontal direction of wind at ground level
	
	//Urho3D::Vector2 m_highWind; // horizontal direction of wind at high
	
	AirContent() : CellContent(CellContent::AIR)
		, m_baseAltitude(0)
		, m_airMass(10000.0)
		, m_waterMass(0)
		, m_volumeLevel(0)
		, m_airPressure(0) // air pressure
		, m_dynamicPressureLow(0) //dynamic pressure
		//, m_dynamicPressureHigh(0)
		, m_actHum(0)
		, m_cloudsHeight(0)
		, m_precip(CellContent::NONE)
		, m_volumeBase(0)
		, m_maxPressureDiff(0)
		, m_maxVolumeDiff(0)
		, m_airMassDiff(0.0)
		, m_waterMassDiff(0)
	{}
};

class GroundContent : public CellContent {
public:
	float m_maxWaterCapacity;
	float m_waterMass; // mass of water on the ground
	
	// helper
	
	float m_waterMassDiff;
	
	
	GroundContent() : CellContent(CellContent::GROUND)
		, m_maxWaterCapacity(0)
		, m_waterMass(0)
		, m_waterMassDiff(0)
	{}
};

#define CLIMATE_LEVELS 2

class ClimateCell
{
	CellContent::ContentType m_lowLevelType;
	ClimateCell *m_neightbours[8];
	CellContent *m_levels[CLIMATE_LEVELS];
	AirContent m_air;
	WaterContent m_water;
	GroundContent m_ground;
public:
	ClimateCell();
	~ClimateCell();
	
	inline ClimateCell* GetNeightbour(int32_t index) const { return m_neightbours[index]; }
	inline void SetNeightbour(int32_t index, ClimateCell* pNeightbour) { m_neightbours[index] = pNeightbour; }
	
	CellContent* GetContent(int32_t level);
	
	const CellContent* GetContent(int32_t level) const;
	
	inline bool IsCheckContent(int32_t level, CellContent::ContentType type) const
	{
		if(level == 0)
		{
			return (CellContent::AIR == type);
		}
		
		if(level == 1)
		{
			return (m_lowLevelType == type);
		}
		
		return false;
	}
	
	inline bool IsCheckContent(CellContent::ContentType type) const
	{
		return (m_lowLevelType == type);
	}
	
	inline AirContent& GetAirContent() { return m_air; }
	inline GroundContent& GetGroundContent() { return m_ground; }
	inline WaterContent& GetWaterContent() { return m_water; }
	
	inline const AirContent& GetAirContent() const { return m_air; }
	
	inline void SetLowContent( CellContent::ContentType contType ) { m_lowLevelType = contType; }
	
};

#endif // __CLIMATE_CELL__H__
