#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdint>
#include <iostream>
#include "ClimateMap.h"

void writeMap(const int8_t *map, int32_t mapSizeX, int32_t mapSizeY);

void writeMap(std::ostream &out, const int8_t *map, int32_t mapSizeX, int32_t mapSizeY);

void writeMapStatistics(std::ostream &out, const uint8_t *map, int32_t mapSizeX, int32_t mapSizeY, int32_t waterLevel);

void writeClimateStatistics(std::ostream &out, ClimateMap &map, int32_t waterLevel);

void writeCellStats(std::ostream &out, ClimateMap &map, int32_t x, int32_t y, int32_t level);

void writeAirStats(std::ostream &out, ClimateCell& cell, int32_t x, int32_t y);
void writeWaterStats(std::ostream &out, ClimateCell& cell, int32_t x, int32_t y);
void writeGroundStats(std::ostream &out, ClimateCell& cell, int32_t x, int32_t y);

#endif //__UTILS_H__