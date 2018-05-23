#include "ClimateCell.h"

ClimateCell::ClimateCell() : m_lowLevelType(CellContent::NONE)
{
}

ClimateCell::~ClimateCell()
{}

CellContent* ClimateCell::GetContent(int32_t level)
{
	if(level == 0)
	{
		return &m_air;
	} 
	
	if((level == 1)&&(m_lowLevelType == CellContent::WATER))
	{
		return &m_water;
	} 
	
	if((level == 1)&&(m_lowLevelType == CellContent::GROUND))
	{
		return &m_ground;
	} 	
	
	return nullptr;
}

const CellContent* ClimateCell::GetContent(int32_t level) const
{
	if(level == 0)
	{
		return &m_air;
	} 
	
	if((level == 1)&&(m_lowLevelType == CellContent::WATER))
	{
		return &m_water;
	} 
	
	if((level == 1)&&(m_lowLevelType == CellContent::GROUND))
	{
		return &m_ground;
	} 	
	
	return nullptr;
}

