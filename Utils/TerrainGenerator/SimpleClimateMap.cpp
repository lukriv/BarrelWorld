#include "SimpleClimateMap.h"

static const int32_t CLIMATE_MAP_SIG = 201844;
static const int32_t CLIMATE_MAP_VERSION = 1;

bool SimpleClimateMap::Load(std::istream& input)
{
	if(!LoadHeader(input, CLIMATE_MAP_SIG, CLIMATE_MAP_VERSION)) return false;
	for (int32_t i = 0; i < m_totalSize; ++i)
	{
		m_pMap[i].Load(input);
	}
	return true;
}

bool SimpleClimateMap::Store(std::ostream& output) const
{
	if(!StoreHeader(output, CLIMATE_MAP_SIG, CLIMATE_MAP_VERSION)) return false;
	for (int32_t i = 0; i < m_totalSize; ++i)
	{
		m_pMap[i].Store(output);
	}
	return true;
}
