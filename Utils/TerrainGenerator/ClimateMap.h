#ifndef __TG_CLIMATE_MAP_H__
#define __TG_CLIMATE_MAP_H__

#include "MapContainer.h"
#include "ClimateCell.h"

class ClimateMap : public MapContainer<ClimateCell, SphereMapCoords>, public SerializableTG
{
public:

	ClimateMap() {}
	~ClimateMap() {}

	virtual	bool Load(std::istream& input) override;
	virtual	bool Store(std::ostream& output) const override;

};

#endif // __TG_CLIMATE_MAP_H__
