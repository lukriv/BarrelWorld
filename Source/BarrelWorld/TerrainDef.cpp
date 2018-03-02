#include "TerrainDef.h"

	const BW::TerrainTexTableRow BW::GLOBAL_TERRAIN_TABLE[] = {
		{TERR_GRASS, "Textures/webGrass.dds"},
		{TERR_ROCK, "Textures/webStone.dds"	},
		{TERR_SNOW, "Textures/webSnow.dds"	},
		{TERR_SAND, "Textures/webSand.dds" 	},
		{TERR_DIRT, "Textures/webDirt.dds"	},
		{TERR_GRASS_OLD, "Textures/webGrassOld.dds"},
		{TERR_BLACK, "Textures/black.dds"	},
		{TERR_UNDEFINED, ""} // the last one
		
	};
	
	
	const BW::TerrainIndexTableRow BW::MODERATE_BELT[] = {
		{TERR_GRASS, 		0},
		{TERR_ROCK, 		1},
		{TERR_SNOW,			2},
		{TERR_SAND,			3},
		{TERR_GRASS_OLD,	4},
		{TERR_BLACK,		5},
		{TERR_UNDEFINED,	0} // the last one
	};