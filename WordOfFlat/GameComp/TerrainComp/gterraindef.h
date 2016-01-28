#ifndef __GAME_TERRAIN_DEFINITION_H__
#define __GAME_TERRAIN_DEFINITION_H__

#include <GameSystem/gdefs.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>

struct TerrainPage : public RefObjectImpl<IRefObject> {
	wxString m_filename;
	wxInt32 m_pageX;
	wxInt32 m_pageY;
};

struct TerrainDef : public RefObjectImpl<IRefObject> {
	wxDword m_mapSize;
	float m_worldSize;
	wxVector< RefObjSmPtr<TerrainPage> > m_terrainPages;
};



#endif //__GAME_TERRAIN_DEFINITION_H__