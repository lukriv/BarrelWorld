#ifndef __BARRELWORLD_TERRAIN_DEFINITIONS__H__
#define __BARRELWORLD_TERRAIN_DEFINITIONS__H__

#include <stdint.h>

namespace BW
{
	
	enum ETerrainType {
		TERR_UNDEFINED = 0,
		TERR_GRASS = 1,
		TERR_ROCK = 2,
		TERR_SNOW = 3,
		TERR_SAND = 4,
		TERR_DIRT = 5,
		TERR_GRASS_OLD = 6,
		TERR_BLACK = 7
	};
	
	//terrain texture table row
	struct TerrainTexTableRow {
		BW::ETerrainType m_texType;
		const char * m_pTextureName;
	};

	// standard texture table
	extern const TerrainTexTableRow GLOBAL_TERRAIN_TABLE[];

	struct TerrainIndexTableRow {
		BW::ETerrainType m_texType;
		int32_t m_index;
	};

	extern const TerrainIndexTableRow MODERATE_BELT[];
}

#endif // __BARRELWORLD_TERRAIN_DEFINITIONS__H__
