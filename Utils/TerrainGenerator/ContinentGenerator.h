#ifndef __CONTITNENT_MAP_GENERATOR__H__
#define __CONTITNENT_MAP_GENERATOR__H__

#include <cstdint>
#include "MapContainer.h"

class ContinentMapGenerator {
public:
	struct Params {
		
		int8_t  m_waterLevel; // water level ( last height where the water is )
		int32_t m_groundPercentage; // percentage of ground
		int32_t m_jitterPercentage; // percentage of jitter
		int32_t m_jitterStrong; // strong of jitter
		int32_t m_chunkSizeMin; // minimum chunksize
		int32_t m_chunkSizeMax; // maximum chunksize
		int32_t m_compactGround; // higher probablity to choose ground tile
		int32_t m_highRiseProb; // high rise probablity (percentages)
		int32_t m_sinkProb; // land sink probabliry (percentages)
		int32_t m_minElev; // elevation minimum
		int32_t m_maxElev; // elevation maximum
		int32_t m_regions; // regions count
		int32_t m_xBorder; // x map border
		int32_t m_yBorder; // y map border
		int32_t m_erosion; // erosion (percentages)
		int32_t m_addMode; // boolean add mode enabled or not
		
	public:
		void clearParams()
		{
			m_waterLevel = 0; // water level ( last height where the water is )
			m_groundPercentage = 0; // percentage of groundparams.m_bornFrom = 0;
			m_jitterPercentage = 0; // jitter in percentage
			m_jitterStrong = 0;
			m_chunkSizeMin = 0;
			m_chunkSizeMax = 0;
			m_compactGround = 0; // higher probablity to choose ground tile
			m_highRiseProb = 0;
			m_sinkProb = 0; //
			m_minElev = 0;
			m_maxElev = 127;
			m_regions = 1;
			m_xBorder = 0;
			m_yBorder = 0;
			m_erosion = 50;
			m_addMode = 1;
		}


		Params()
		{
			clearParams();
		}
		
	};


	static void GenerateMap(MapContainer<int8_t, SphereMapCoords>& map, const Params& params);
	
	static void ErodeMap(MapContainer<int8_t, SphereMapCoords>& map, const Params& params);
	
};

#endif //__CONTITNENT_MAP_GENERATOR__H__
