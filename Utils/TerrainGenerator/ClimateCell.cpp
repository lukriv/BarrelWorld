#include "ClimateCell.h"

ClimateCell::ClimateCell()
{
	for(int32_t i = 0; i < CLIMATE_LEVELS; ++i)
	{
		m_levels[i] = nullptr;
	}
}

ClimateCell::~ClimateCell()
{
	for(int32_t i = 0; i < CLIMATE_LEVELS; ++i)
	{
		if(m_levels[i] != nullptr)
		{
			delete m_levels[i];
			m_levels[i] = nullptr;
		}
	}
}

CellContent* ClimateCell::GetContent(int32_t level)
{
	if((level < 0)||(level >= CLIMATE_LEVELS))
	{
		return nullptr;
	}
	return m_levels[level];
}

CellContent* ClimateCell::GetContent(int32_t level) const
{
	if((level < 0)||(level >= CLIMATE_LEVELS))
	{
		return nullptr;
	}
	return m_levels[level];
}

bool ClimateCell::IsCheckContent(int32_t level, CellContent::ContentType type) const
{
	if((level < 0)||(level >= CLIMATE_LEVELS))
	{
		return false;
	}
	
	if(m_levels[level] != nullptr)
	{
		return (m_levels[level]->GetContentType() == type);
	}
	
	return false;
}

AirContent* ClimateCell::GetAirContent(int32_t level)
{
	if(IsCheckContent(level, CellContent::AIR))
	{
		return reinterpret_cast<AirContent*>(m_levels[level]);
	}
	
	return nullptr;
}

AirContent* ClimateCell::GetAirContent(int32_t level) const
{
	if(IsCheckContent(level, CellContent::AIR))
	{
		return reinterpret_cast<AirContent*>(m_levels[level]);
	}
	
	return nullptr;
}

GroundContent* ClimateCell::GetGroundContent(int32_t level)
{
	if(IsCheckContent(level, CellContent::GROUND))
	{
		return reinterpret_cast<GroundContent*>(m_levels[level]);
	}
	
	return nullptr;
}

WaterContent* ClimateCell::GetWaterContent(int32_t level)
{
	if(IsCheckContent(level, CellContent::WATER))
	{
		return reinterpret_cast<WaterContent*>(m_levels[level]);
	}
	
	return nullptr;
}

void ClimateCell::AddContent(int32_t level, CellContent* pContent)
{
	if((level < 0)||(level >= CLIMATE_LEVELS))
	{
		return;
	}
	
	if(m_levels[level] != nullptr)
	{
		delete m_levels[level];
	}
	
	m_levels[level] = pContent;
}



