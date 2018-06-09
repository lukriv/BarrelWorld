#include "ContinentGenerator.h"

#include <random>
#include <queue>
#include <chrono>
#include <cstring>

#include <iostream>

#include "global.h"

//#define _SDL_DEBUG_DETAIL_
// #define _SDL_DEBUG_

struct MapNode {
	int32_t x_;
	int32_t y_;
	float m_distance;
	
	MapNode() :x_(-1), y_(-1), m_distance(0) {}
	
	MapNode(int32_t x, int32_t y, float distance) :x_(x), y_(y), m_distance(distance) {}
};

struct Region {
	int32_t m_xMin;
	int32_t m_xMax;
	int32_t m_yMin;
	int32_t m_yMax;
	
	Region() :m_xMin(0), m_xMax(0), m_yMin(0), m_yMax(0) {}
	Region(int32_t xMin, int32_t xMax, int32_t yMin, int32_t yMax) : m_xMin(xMin), m_xMax(xMax), m_yMin(yMin), m_yMax(yMax) {}

	int32_t GetSize() const
	{
		return ((m_xMax - m_xMin) * (m_yMax - m_yMin));
	}
};

std::ostream& operator<< (std::ostream& os, const Region& reg)
{
	os << "Region x[" << reg.m_xMin << ", " << reg.m_xMax << "] y[" << reg.m_yMin << ", " << reg.m_yMax << "]"; 
	return os;
}

bool IsErodible(HeightMap& map, int32_t ix, int32_t iy)
{
	int32_t nx, ny;
	uint8_t cElev = map.GetCellValue(ix, iy);
	for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
	{
		map.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), ix, iy, nx, ny);
		
		
		uint8_t nElev = map.GetCellValue(nx, ny);
		
		if ( (cElev - nElev) > 1 )
		{
			return true;
		}
	}
	
	return false;
}

void ContinentMapGenerator::GenerateMap(HeightMap& map, const ContinentMapGenerator::Params& params)
{
	int32_t totalMapSize = map.GetSizeX()*map.GetSizeY();
	HeightMap searchMap;
	searchMap.Initialize(map.GetSizeX(), map.GetSizeY());
	
	map.FillMap(0);
	
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> fdist(0.0f,1.0f);
	std::uniform_real_distribution<float> regRatioDist(0.2f,0.8f); // region ration distribution 
	std::uniform_real_distribution<float> jitterDist(1.0f,(float)params.m_jitterStrong);
	std::uniform_int_distribution<int32_t> totalDist(params.m_chunkSizeMin, params.m_chunkSizeMax);
	
	struct mycomparison
	{
		bool operator() (const MapNode& lhs, const MapNode& rhs) 
		{ return (lhs.m_distance > rhs.m_distance);}
	};
	
	std::priority_queue<MapNode, std::vector<MapNode>, mycomparison > searchFrontier;
	std::queue<Region> regionQueue;
	//std::queue<MapNode*> searchFrontier;
	int32_t totalSize = (totalMapSize*(params.m_groundPercentage%100))/100;
	int32_t chunkSize = 0;
	int32_t size = 0;
	int8_t searchFrontierPhase = 1; // first phase
	float jitter = ((float)params.m_jitterPercentage)/100.0f;
	float highRise = ((float)params.m_highRiseProb)/100.0f;
	float sink = ((float)params.m_sinkProb)/100.0f;
	
	int32_t x = 0;
	int32_t y = 0;
	int32_t tries = 0;
	int8_t rise = 1;
	
	int32_t totalCycles = 0;
	int32_t highRiseCount = 0;
	int32_t sinkCount = 0;
	int32_t regions = 0;
	
	// Create regions
	regionQueue.push(Region(0,map.GetSizeX(),0,map.GetSizeY()));
	
	float regionDividing = 0.5;
	
	while (regionQueue.size() < (size_t)params.m_regions)
	{
		// divide first region
		const Region &reg = regionQueue.front();
		Region a,b;
		float ratio = regRatioDist(generator);
		
		regionDividing = (float)(reg.m_xMax - reg.m_xMin) / (float)((reg.m_xMax - reg.m_xMin)+(reg.m_yMax - reg.m_yMin));
		
		std::cout << "Divided region: " << reg << ", ratio : " << regionDividing << std::endl;
		
		if(0.5 > regionDividing)
		{
			//horizontal divide
			a.m_xMin = b.m_xMin = reg.m_xMin;
			a.m_xMax = b.m_xMax = reg.m_xMax;
			
			int32_t borderline = (int32_t)((float)(reg.m_yMax - reg.m_yMin) * ratio) + reg.m_yMin;
			
			a.m_yMin = reg.m_yMin;
			a.m_yMax = borderline;
			
			b.m_yMin = borderline;
			b.m_yMax = reg.m_yMax;
			
		} else {
			// vertical divide
			a.m_yMin = b.m_yMin = reg.m_yMin;
			a.m_yMax = b.m_yMax = reg.m_yMax;
			
			int32_t borderline = (int32_t)((float)(reg.m_xMax - reg.m_xMin) * ratio) + reg.m_xMin;
			
			a.m_xMin = reg.m_xMin;
			a.m_xMax = borderline;
			
			b.m_xMin = borderline;
			b.m_xMax = reg.m_xMax;
		}
		
		if(a.GetSize() > b.GetSize())
		{
			regionQueue.push(a);
			std::cout << "Add region: " << a << std::endl;
			regionQueue.push(b);
			std::cout << "Add region: " << b << std::endl;
		} else {
			regionQueue.push(b);
			std::cout << "Add region: " << b << std::endl;
			regionQueue.push(a);
			std::cout << "Add region: " << a << std::endl;
		}
		
		// pop front - divided region
		regionQueue.pop();
		
	}
	
	// generate land
	while ((totalSize > 0) && (totalCycles < 10000) && (regions < params.m_regions) && (!regionQueue.empty()))
	{
		
		const Region &reg = regionQueue.front();
		
		Region rg(reg.m_xMin + params.m_xBorder,reg.m_xMax - 1 - params.m_xBorder, reg.m_yMin + params.m_yBorder,reg.m_yMax - 1 - params.m_yBorder);
		
		int32_t regionTotalSize = reg.GetSize() *(params.m_groundPercentage%100)/100;
		
		std::cout << "Original: " << reg << ", region size: " << regionTotalSize << std::endl;
		std::cout << " With borders: " << rg << std::endl;

		regionQueue.pop();
		if((rg.m_xMin >= rg.m_xMax)||(rg.m_yMin >= rg.m_yMax))
		{
			continue;
		}
		
		++regions;
		
		std::uniform_int_distribution<int32_t> distX(rg.m_xMin,rg.m_xMax);
		std::uniform_int_distribution<int32_t> distY(rg.m_yMin,rg.m_yMax);
		
		while ((totalSize > 0) && (totalCycles < 10000) && (regionTotalSize > 0))
		{
			
			chunkSize = totalDist(generator);
			
			size = 0;
			
			x = distX(generator);
			y = distY(generator);
			
			int8_t level = map.GetCellValue(x,y);
			
			if((level <= params.m_waterLevel) && (tries < params.m_compactGround))
			{
				++tries;
				continue;
			} else {
				tries = 0;
			}
			
			searchFrontier.push(MapNode(x,y,0));
			
			searchMap.SetCellValue(x,y,searchFrontierPhase);
			
			rise = ( fdist(generator) < highRise ) ? 2 : 1;
			rise = ( fdist(generator) < sink ) ? -rise : rise;
			
			++totalCycles;
			
			if(std::abs(rise) != 1)
			{
				++highRiseCount;
			}
			
			if(rise < 0)
			{
				++sinkCount;
			}
			
			
			
			while ((size < chunkSize) && (searchFrontier.size() > 0) && (totalSize > 0))
			{
				MapNode current = searchFrontier.top();
				//const MapNode *current = searchFrontier.front();
				searchFrontier.pop();
				
				int8_t oldlevel = map.GetCellValue(current.x_,current.y_);
				int8_t newlevel = 0;
				if(params.m_addMode)
				{
					newlevel = oldlevel + rise;
				} else {
					newlevel = params.m_waterLevel + 1;
				}
				
				// constrain new level
				newlevel = (newlevel > params.m_maxElev) ? params.m_maxElev : newlevel;
				newlevel = (newlevel < params.m_minElev) ? params.m_minElev : newlevel;
								
				map.SetCellValue(current.x_, current.y_, newlevel);
				
				size += 1;
				
				// is ground? (underwater tiles does not count)
				if((oldlevel <= params.m_waterLevel)&&(newlevel > params.m_waterLevel))
				{
					totalSize -= 1;
					regionTotalSize -= 1;
				}
				
				// sink case
				if((oldlevel > params.m_waterLevel)&&(newlevel <= params.m_waterLevel))
				{
					totalSize += 1;
					regionTotalSize += 1;
				}
				
				int32_t nx, ny;
				for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
				{
					map.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), current.x_, current.y_, nx, ny);
					
					int8_t searchPhase = searchMap.GetCellValue(nx, ny);
					
					if (searchPhase < searchFrontierPhase) 
					{
						searchMap.SetCellValue(nx,ny,searchFrontierPhase);
						//float add = ((std::abs(xDiff)+std::abs(yDiff)) > 1)? 1.4f : 1.0f;
						float add = 1.0f;
						float randx = (fdist(generator) < jitter) ? jitterDist(generator) : 0.0;
						float distance = current.m_distance + add + randx;
						searchFrontier.push(MapNode(nx,ny,distance));
					}
				}
		#ifdef _SDL_DEBUG_DETAIL_			
				gSDL->writeMap(pMap, mapSizeX, mapSizeY, params.m_waterLevel);
				SDLutils::ACTION act = gSDL->waitForAction();
				
				while ((act != SDLutils::ACTION::NEXT)&&(act != SDLutils::ACTION::QUIT))
				{
					act = gSDL->waitForAction();
				}
				
				if(act == SDLutils::ACTION::QUIT)
				{
					return;
				}
		#endif // _SDL_DEBUG_DETAIL_
					
				
				
			}

			// clear queue
			searchFrontier = std::priority_queue<MapNode, std::vector<MapNode>, mycomparison >();
			// clear search
			searchMap.FillMap(0);
		
		
		}
		
#ifdef _SDL_DEBUG_
		gSDL->writeMap(map.GetData(), map.GetSizeX(), map.GetSizeY(), params.m_waterLevel);
		SDLutils::ACTION act = gSDL->waitForAction();
		
		while ((act != SDLutils::ACTION::NEXT)&&(act != SDLutils::ACTION::QUIT))
		{
			act = gSDL->waitForAction();
		}
		
		if(act == SDLutils::ACTION::QUIT)
		{
			return;
		}
#endif // _SDL_DEBUG_
		
	}
	
	std::cout << "Total cycles: " << totalCycles << std::endl;
	std::cout << "High rise (%): " << (highRiseCount*100)/(totalCycles + 1) << std::endl;
	std::cout << "Sink (%): " << (sinkCount*100)/(totalCycles + 1) << std::endl;
	

	
	
	
}

void ContinentMapGenerator::ErodeMap(HeightMap& map, const Params& params)
{
	// skip erosion if it is not desired
	if(params.m_erosion == 0) return;
	
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> fdist(0.0f,1.0f);
	
	// erosion
	// find erodable cells 
	HeightMap searchMap;
	searchMap.Initialize(map.GetSizeX(), map.GetSizeY());
	std::queue<MapNode> erodibleCells;
	
	float erosionProb = ((float)params.m_erosion)/100.0f;
	
	for (int32_t iy = 0; iy < map.GetSizeY(); ++iy)
	{
		for (int32_t ix = 0; ix < map.GetSizeX(); ++ix)
		{
			if(IsErodible(map,ix,iy))
			{
				erodibleCells.push(MapNode(ix, iy, 0.0f));
				searchMap.SetCellValue(ix,iy,(int8_t)1);
			}
		}
	}
	
	std::cout << "Erodible cells : " << erodibleCells.size() << std::endl;
	
	while(!erodibleCells.empty())
	{
		const MapNode& current = erodibleCells.front();
		if(erosionProb > fdist(generator))
		{
			// erode cell
			int8_t elevation = map.GetCellValue(current.x_, current.y_) - 1;
			map.SetCellValue(current.x_, current.y_, elevation);
			
			// find new erodable cell
			int32_t nx, ny;
			for (int32_t dir = static_cast<int32_t>(MapContainerDIR::N); dir <= static_cast<int32_t>(MapContainerDIR::NE); ++dir) 
			{
				map.GetNeightbourCoords(static_cast<MapContainerDIR>(dir), current.x_, current.y_, nx, ny);
				
				if(( searchMap.GetCellValue(nx, ny) == 0) && IsErodible(map,nx,ny))
				{
					erodibleCells.push(MapNode(nx, ny, 0.0f));
					searchMap.SetCellValue(nx,ny, (int8_t)1);
				}
			}
			
			// is current still erodible?
			if(IsErodible(map,current.x_,current.y_))
			{
				erodibleCells.push(MapNode(current));
			}
		}
		
		// remove current from queue
		erodibleCells.pop();
		
	}
}
