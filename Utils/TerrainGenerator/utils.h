#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdint>
#include <iostream>
#include "MapContainer.h"
#include "ClimateCell.h"

void writeMap(const int8_t *map, int32_t mapSizeX, int32_t mapSizeY);

void writeMap(std::ostream &out, const int8_t *map, int32_t mapSizeX, int32_t mapSizeY);

void writeMapStatistics(std::ostream &out, const uint8_t *map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel);

void writeClimateStatistics(std::ostream &out, MapContainer<ClimateCell, SphereMapCoords> &map, int32_t waterLevel);


#endif //__UTILS_H__