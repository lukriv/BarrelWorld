#ifndef __SIMPLE_CLIMATE_CELL__H__
#define __SIMPLE_CLIMATE_CELL__H__

#include "SerializableTG.h"

class SimpleClimateCell : public SerializableTG
{
	float m_cloads;
public:

	SimpleClimateCell() {}
	virtual ~SimpleClimateCell() {}

	virtual	bool Load(std::istream& input);
	virtual	bool Store(std::ostream& output) const;
};

#endif // __SIMPLE_CLIMATE_CELL__H__
