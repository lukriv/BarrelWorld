#ifndef __CLIMATE_CELL__H__
#define __CLIMATE_CELL__H__

#include <Urho3D/Math/Vector2.h>
#include <cstdint>
#include "SerializableTG.h"


const float CLIMATE_MIN_TEMPERATURE = -50;
const float CLIMATE_MIN_TEMPERATURE_KELVIN = -50 + 273;
const float CLIMATE_MAX_TEMPERATURE = 150;
const float CLIMATE_MAX_TEMPERATURE_KELVIN = 150 + 273;

class CellContent : public SerializableTG
{
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
	ContentType GetContentType() const
	{
		return m_type;
	}
};

class WaterContent : public CellContent
{
public:
	float m_saltMass;
	float m_waterMass;
	float m_iceMass;
	float m_evapWaterMass; // evaporated water mass
	
	// updatable
	//float m_dynamicPressure; // dynamic pressure ( from Bernoulli's principle )
	//float m_surfaceLevel; // for water surface level difference

	// helper
	float m_waterMassDiff;
	float m_iceMassDiff;

	//float m_maxSurfaceDiff;

	//Urho3D::Vector2 m_dynamicForce; // force to stream move

	//Urho3D::Vector2 m_streamDir; // vector of water stream

	WaterContent() : CellContent(CellContent::WATER)
		, m_saltMass(0)
		, m_waterMass(0)
		, m_iceMass(0)
		, m_evapWaterMass(0)
		//, m_dynamicPressure(0)
		//, m_surfaceLevel(0)
		, m_waterMassDiff(0)
		, m_iceMassDiff(0)
	{}
	
	virtual	bool Load(std::istream& input) override
	{
		SerializableTG::Load( input, m_temperature);
		SerializableTG::Load( input, m_waterMass);
		SerializableTG::Load( input, m_iceMass);
		//SerializableTG::Load( input, m_streamDir.x_);
		//SerializableTG::Load( input, m_streamDir.y_);
		return true;
	}
	
	virtual	bool Store(std::ostream& output) const override
	{
		SerializableTG::Store( output, m_temperature);
		SerializableTG::Store( output, m_waterMass);
		SerializableTG::Store( output, m_iceMass);
		//SerializableTG::Store( output, m_streamDir.x_);
		//SerializableTG::Store( output, m_streamDir.y_);
		return true;
	}


};

class AirContent : public CellContent
{
public:
	float m_baseAltitude;
	float m_airMass; // air mass [kg]
	float m_waterMass; // kilogram of water as vapour [kg]
	
	// updatable
	float m_airPressure; // air pressure at altitude 0 (can be computed from air mass ) [Pa]
	float m_volumeLevel; // volume level at measure level
	float m_dynamicPressureLow; // dynamic pressure ( from Bernoulli's principle )
	float m_dynamicPressureHigh; // dynamic pressure ( from Bernoulli's principle )
	float m_actHum; // actual air humidity [kg/kg]
	float m_cloudsHeight; // altitude of clouds
	CellContent::ContentType m_precip; // precipitation type

	// helper
	//float m_volumeBase; // initial base volume constant
	float m_maxPressureDiff; // maximal pressure difference between neightbour cells
	float m_maxVolumeDiff; // maximal volume difference between neightbour cells


	float m_airMassDiff; // air mass difference
	float m_waterMassDiff;

	Urho3D::Vector2 m_dynamicForce;

	Urho3D::Vector2 m_dynamicForceHigh;

	//Urho3D::Vector2 m_lowForce; // force direction between neightbour cells

	//Urho3D::Vector2 m_highForce; // high force direction between neightbour cells

	Urho3D::Vector2 m_lowWind; // horizontal direction of wind at ground level

	Urho3D::Vector2 m_highWind; // horizontal direction of wind at high

	AirContent() : CellContent(CellContent::AIR)
		, m_baseAltitude(0)
		, m_airMass(10000.0)
		, m_waterMass(0)
		, m_airPressure(0) // air pressure
		, m_volumeLevel(0)
		, m_dynamicPressureLow(0) //dynamic pressure
		, m_dynamicPressureHigh(0)
		, m_actHum(0)
		, m_cloudsHeight(0)
		, m_precip(CellContent::NONE)
		//, m_volumeBase(0)
		, m_maxPressureDiff(0)
		, m_maxVolumeDiff(0)
		, m_airMassDiff(0.0)
		, m_waterMassDiff(0)
	{}
	
	virtual	bool Load(std::istream& input) override
	{
		int32_t prec = 0;
		
		SerializableTG::Load(input, m_temperature);
		SerializableTG::Load(input, m_airMass);
		SerializableTG::Load(input, m_baseAltitude);
		SerializableTG::Load(input, m_waterMass);
		SerializableTG::Load(input, prec);
		SerializableTG::Load(input, m_lowWind.x_);
		SerializableTG::Load(input, m_lowWind.y_);
		SerializableTG::Load(input, m_highWind.x_);
		SerializableTG::Load(input, m_highWind.y_);		
		
		
		m_precip = static_cast<CellContent::ContentType>(prec);
		return true;
	}
	
	virtual	bool Store(std::ostream& output) const override
	{
		SerializableTG::Store(output, m_temperature);
		SerializableTG::Store(output, m_airMass);
		SerializableTG::Store(output, m_baseAltitude);
		SerializableTG::Store(output, m_waterMass);
		SerializableTG::Store(output, m_precip);
		SerializableTG::Store(output, m_lowWind.x_);
		SerializableTG::Store(output, m_lowWind.y_);
		SerializableTG::Store(output, m_highWind.x_);
		SerializableTG::Store(output, m_highWind.y_);
		return true;
	}

};

class GroundContent : public CellContent
{
public:
	float m_waterMass; // mass of water on the ground
	int32_t m_riverDir; // river direction
	// helper
	float m_waterFlow; // how much water flow every cycle
	int32_t m_waterFlowCont; // count of cycles where the flow was greater than zero
	float m_waterMassDiff;


	GroundContent() : CellContent(CellContent::GROUND)
		, m_waterMass(0)
		, m_riverDir(-1)
		, m_waterFlow(0)
		, m_waterFlowCont(0)
		, m_waterMassDiff(0)
	{}
	
	virtual	bool Load(std::istream& input) override
	{
		SerializableTG::Load( input, m_temperature);
		SerializableTG::Load( input, m_waterMass);
		SerializableTG::Load( input, m_riverDir);
		//SerializableTG::Load( input, m_waterFlow);
		//SerializableTG::Load( input, m_waterFlowCont);
		return true;
	}
	
	virtual	bool Store(std::ostream& output) const override
	{
		SerializableTG::Store(output, m_temperature);
		SerializableTG::Store(output, m_waterMass);
		SerializableTG::Store(output, m_riverDir);
		//SerializableTG::Store(output, m_waterFlow);
		//SerializableTG::Store(output, m_waterFlowCont);
		return true;
	}

};

#define CLIMATE_LEVELS 2

class ClimateCell : public SerializableTG
{
	CellContent::ContentType m_lowLevelType;
	ClimateCell *m_neightbours[8];
	CellContent *m_levels[CLIMATE_LEVELS];
	AirContent m_air;
	WaterContent m_water;
	GroundContent m_ground;
public:

	ClimateCell();
	virtual ~ClimateCell();

	inline ClimateCell* GetNeightbour(int32_t index) const
	{
		return m_neightbours[index];
	}
	inline void SetNeightbour(int32_t index, ClimateCell* pNeightbour)
	{
		m_neightbours[index] = pNeightbour;
	}

	CellContent* GetContent(int32_t level);

	const CellContent* GetContent(int32_t level) const;

	inline bool IsCheckContent(int32_t level, CellContent::ContentType type) const
	{
		if(level == 0) {
			return (CellContent::AIR == type);
		}

		if(level == 1) {
			return (m_lowLevelType == type);
		}

		return false;
	}

	inline bool IsCheckContent(CellContent::ContentType type) const
	{
		return (m_lowLevelType == type);
	}

	inline AirContent& GetAirContent()
	{
		return m_air;
	}
	inline GroundContent& GetGroundContent()
	{
		return m_ground;
	}
	inline WaterContent& GetWaterContent()
	{
		return m_water;
	}

	inline const AirContent& GetAirContent() const
	{
		return m_air;
	}
	
	inline const GroundContent& GetGroundContent() const
	{
		return m_ground;
	}

	inline void SetLowContent( CellContent::ContentType contType )
	{
		m_lowLevelType = contType;
	}

	virtual	bool Load(std::istream& input) override;
	virtual	bool Store(std::ostream& output) const override;

};

#endif // __CLIMATE_CELL__H__
