#ifndef __SIMPLE_CLIMATE_CELL__H__
#define __SIMPLE_CLIMATE_CELL__H__

class SimpleClimateCell : public SerializableTG
{
public:

	SimpleClimateCell() {}
	~SimpleClimateCell() {}

	virtual	bool Load(std::istream& input);
	virtual	bool Store(std::ostream& output) const;
};

#endif // __SIMPLE_CLIMATE_CELL__H__
