#ifndef __CELLULAR_AUTOMATON_MAP_GENERATOR_H__
#define __CELLULAR_AUTOMATON_MAP_GENERATOR_H__

#include <cstdint>
#include "MapContainer.h"



class CellularAutomatonMapGenerator {
public:
	struct Params {
		int8_t m_growsOn;
		int8_t m_lifeForm;
		int32_t m_initialPercentage;
		int32_t m_bornFrom;
		int32_t m_bornTo;
		int32_t m_aliveFrom;
		int32_t m_aliveTo;
		int32_t m_iterations;
		
		void clearParams()
		{
			m_growsOn = 0;
			m_lifeForm = 0;
			m_initialPercentage = 0;
			m_bornFrom = 0;
			m_bornTo = 0;
			m_aliveFrom = 0;
			m_aliveTo = 0;
			m_iterations = 0;
		}
		
		Params()
		{
			clearParams();
		}
		
	};

	static void GenerateMap(MapContainer<uint8_t, SphereMapCoords> &map, const Params& params);
};

#endif //__CELLULAR_AUTOMATON_MAP_GENERATOR_H__