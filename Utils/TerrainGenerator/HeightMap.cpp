#include "HeightMap.h"

static const int32_t HEIGHT_MAP_SIG = 201801;
static const int32_t HEIGHT_MAP_VERSION = 1;

bool HeightMap::Load(std::istream& input)
{
	if(!LoadHeader(input, HEIGHT_MAP_SIG, HEIGHT_MAP_VERSION)) return false;
	for (int32_t i = 0; i < m_totalSize; ++i)
	{
		SerializableTG::Load(input, m_pMap[i]);
	}
	
	return true;
	
}

bool HeightMap::Store(std::ostream& output) const
{
	if(!StoreHeader(output, HEIGHT_MAP_SIG, HEIGHT_MAP_VERSION)) return false;
	for (int32_t i = 0; i < m_totalSize; ++i)
	{
		SerializableTG::Store(output, m_pMap[i]);
	}
	
	return true;
	
}
