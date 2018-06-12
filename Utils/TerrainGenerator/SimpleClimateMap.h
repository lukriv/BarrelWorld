#ifndef __SIMPLE_CLIMATE_MAP__H__
#define __SIMPLE_CLIMATE_MAP__H__

#include "MapContainer.h"
#include "SimpleClimateCell.h"


class SimpleClimateMap : public MapContainer<SimpleClimateCell, SphereMapCoords>, public SerializableTG
{
public:
	SimpleClimateMap() {}
	virtual ~SimpleClimateMap() {}
	
	virtual	bool Load(std::istream& input) override;
	virtual	bool Store(std::ostream& output) const override;
};

#endif // __SIMPLE_CLIMATE_MAP__H__
