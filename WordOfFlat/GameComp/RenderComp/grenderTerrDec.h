#ifndef __GAME_RENDER_TERRAIN_DECAL_H__
#define __GAME_RENDER_TERRAIN_DECAL_H__


#include "grendercomp.h"



class RenderTerrainDecal : public RenderComponent
{
	Ogre::ManualObject *m_pManObject;
	Ogre::RaySceneQuery *m_pRayQuery;

public:
	RenderTerrainDecal(RenderCompManager *pRenderMgr);
	~RenderTerrainDecal();
	
	GameErrorCode Initialize();

	GameErrorCode Create(const wxString& materialName);
	
	void SetNewPosition(Ogre::Real x, Ogre::Real y);
	
	GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);

protected:
	Ogre::Real GetTerrainHeight(Ogre::Real x, Ogre::Real y);
};

#endif // __GAME_RENDER_TERRAIN_DECAL_H__
