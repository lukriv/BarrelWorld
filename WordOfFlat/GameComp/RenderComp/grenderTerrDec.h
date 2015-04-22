#ifndef __GAME_RENDER_TERRAIN_DECAL_H__
#define __GAME_RENDER_TERRAIN_DECAL_H__


#include "grendercomp.h"


class RenderTerrainDecal : public RenderComponent
{
public:
	RenderTerrainDecal();
	~RenderTerrainDecal();

	GameErrorCode Create(const wxString& materialName);
	
	GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);

};

#endif // __GAME_RENDER_TERRAIN_DECAL_H__
