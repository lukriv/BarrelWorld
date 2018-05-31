#ifndef __MAP_CONTAINER_H__
#define __MAP_CONTAINER_H__


#include <cstdint>
#include <cmath>
#include <cstring>
#include <new>

struct ToroidMapCoords {
	void operator()(int32_t mapSizeX, int32_t mapSizeY, int32_t &x, int32_t &y) const
	{
		if(x < 0) x = mapSizeX - (std::abs(x)%mapSizeX);
		if(x >= mapSizeX) x = x%mapSizeX;
		if(y < 0) y = mapSizeY - (std::abs(y)%mapSizeY);
		if(y >= mapSizeY) y = y%mapSizeY;
	}
};

struct SphereMapCoords {
	void operator()(int32_t mapSizeX, int32_t mapSizeY, int32_t &x, int32_t &y) const
	{
		if(x < 0) x = mapSizeX - (std::abs(x)%mapSizeX);
		if(x >= mapSizeX) x = x%mapSizeX;
		//x = (x+mapSizeX)%mapSizeX;
		if(y < 0)
		{
			y = std::abs(y+1)%mapSizeY;
			x = (x + (mapSizeX/2)) % mapSizeX;
		}
		
		
		if(y >= mapSizeY)
		{
			y = (mapSizeY - 1) - (y%mapSizeY);
			x = (x + (mapSizeX/2)) % mapSizeX;
		 
		}
	}
};

enum MapContainerDIR {
		N 	= 0,
		NW 	= 1,
		W	= 2,
		SW	= 3,
		S	= 4,
		SE	= 5,
		E	= 6,
		NE	= 7
	};
	
template<typename TCellValue, class MapCoords>
class MapContainer
{
private:
	int32_t m_sizeX;
	int32_t m_sizeY;
	int32_t m_totalSize;
	TCellValue *m_pMap;
	MapCoords FnMapCoords;
public:
	static void GetNeightbourCoords( MapContainerDIR neighbourDir, int32_t &xDiff, int32_t &yDiff)
	{
		static int32_t vx[] = {  0, -1, -1, -1,  0,  1,  1,  1 };
		static int32_t vy[] = { -1, -1,  0,  1,  1,  1,  0, -1 };
		
		xDiff = vx[neighbourDir];
		yDiff = vy[neighbourDir];
	}
	
public:
	MapContainer() : m_sizeX(0), m_sizeY(0), m_totalSize(0), m_pMap(nullptr) {}
	~MapContainer()
	{
		Release();
	}
	
	void GetNeightbourCoords( MapContainerDIR neighbourDir, int32_t cx, int32_t cy, int32_t &nx, int32_t &ny)
	{
		int32_t xDiff, yDiff;
		GetNeightbourCoords(neighbourDir,xDiff,yDiff);
		nx = cx + xDiff;
		ny = cy + yDiff;
		FnMapCoords(m_sizeX, m_sizeY, nx, ny);
	}
	
	bool Initialize(int32_t x, int32_t y)
	{
		if(m_pMap)
		{
			Release();
		}
		
		m_pMap = new (std::nothrow) TCellValue[x*y];
		m_sizeX = x;
		m_sizeY = y;
		m_totalSize = x*y;
		return true;
	}
	
	bool FillMap(TCellValue value)
	{
		if(!m_pMap)
		{
			return false;
		}
		
		for (int32_t i = 0; i < m_totalSize; ++i)
		{
			m_pMap[i] = value;
		}
		
		return true;
	}
	
	void Release()
	{
		if(m_pMap)
		{
			delete m_pMap;
			m_pMap = nullptr;
		}
		
		
		m_sizeX = 0;
		m_sizeY = 0;
		m_totalSize = 0;
	}
	
	int32_t GetSizeX() const { return m_sizeX; }
	int32_t GetSizeY() const { return m_sizeY; }
	TCellValue* GetData() { return m_pMap; };
	
	void SetCellValue(int32_t x, int32_t y, TCellValue value)
	{
		FnMapCoords(m_sizeX, m_sizeY, x, y);	
		*(m_pMap + y*m_sizeX + x) = value;
	}
	
	TCellValue& GetCellValue(int32_t x, int32_t y)
	{
		FnMapCoords(m_sizeX, m_sizeY, x, y);
		return *(m_pMap + y*m_sizeX + x);
	}
	
};

#endif // __MAP_CONTAINER_H__
