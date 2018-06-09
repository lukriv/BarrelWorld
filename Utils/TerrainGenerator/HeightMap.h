#ifndef __TERRAIN_GENERATOR_HEIGHT_MAP_H__
#define __TERRAIN_GENERATOR_HEIGHT_MAP_H__

#include "MapContainer.h"
#include "SerializableTG.h"

class HeightMap : public MapContainer<uint8_t, SphereMapCoords>, public SerializableTG
{
public:
	HeightMap() {}
	~HeightMap() {}


	virtual	bool Load(std::istream& input) override;
	virtual	bool Store(std::ostream& output) const override;

};

#endif // __TERRAIN_GENERATOR_HEIGHT_MAP_H__
