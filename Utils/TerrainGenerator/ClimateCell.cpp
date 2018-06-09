#include "ClimateCell.h"

ClimateCell::ClimateCell() : m_lowLevelType(CellContent::NONE)
{
}

ClimateCell::~ClimateCell()
{}

CellContent* ClimateCell::GetContent(int32_t level)
{
	if(level == 0) {
		return &m_air;
	}

	if((level == 1)&&(m_lowLevelType == CellContent::WATER)) {
		return &m_water;
	}

	if((level == 1)&&(m_lowLevelType == CellContent::GROUND)) {
		return &m_ground;
	}

	return nullptr;
}

const CellContent* ClimateCell::GetContent(int32_t level) const
{
	if(level == 0) {
		return &m_air;
	}

	if((level == 1)&&(m_lowLevelType == CellContent::WATER)) {
		return &m_water;
	}

	if((level == 1)&&(m_lowLevelType == CellContent::GROUND)) {
		return &m_ground;
	}

	return nullptr;
}

bool ClimateCell::Load(std::istream& input)
{
	int32_t lowLevelType = 0;
	
	SerializableTG::Load(input, lowLevelType);
	m_lowLevelType = static_cast<CellContent::ContentType>(lowLevelType);
	
	if(!m_air.Load(input)) return false;
	if(m_lowLevelType == CellContent::WATER)
	{
		if(!m_water.Load(input)) return false;
	} else {
		if(!m_ground.Load(input)) return false;
	}
	
	return true;
}

bool ClimateCell::Store(std::ostream& output) const
{
	
	SerializableTG::Store( output, (int32_t)m_lowLevelType);
	if(!m_air.Store(output)) return false;
	if(m_lowLevelType == CellContent::WATER)
	{
		if(!m_water.Store(output)) return false;
	} else {
		if(!m_ground.Store(output)) return false;
	}
	return true;
}
