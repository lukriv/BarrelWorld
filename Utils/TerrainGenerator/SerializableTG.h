#ifndef __TERRAIN_GENERATOR_SERIALIZABLE__H__
#define __TERRAIN_GENERATOR_SERIALIZABLE__H__

#include <iostream>

class SerializableTG
{
public:

	static void Load(std::istream &input, uint8_t &value)
	{
		
		input.read((char*)(&value), 1);
	}
	
	static void Store(std::ostream &output, uint8_t value)
	{
		output.write((char*)&value, 1);
	}

	static void Load(std::istream &input, int32_t &value)
	{
		
		input.read((char*)(&value), 4);
	}
	
	static void Store(std::ostream &output, int32_t value)
	{
		output.write((char*)&value, 4);
	}
	
	static void Load(std::istream &input, float &value)
	{
		input.read((char*)(&value), 4);
	}
	
	static void Store(std::ostream &output, float value)
	{
		output.write((char*)&value, 4);
	}


	virtual bool Load(std::istream &input) = 0;
	virtual bool Store(std::ostream &output) const = 0;

};

#endif // __TERRAIN_GENERATOR_SERIALIZABLE__H__
