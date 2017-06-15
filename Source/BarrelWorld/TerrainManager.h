#ifndef __BARRELWORLD_TERRAIN_MANAGER__H__
#define __BARRELWORLD_TERRAIN_MANAGER__H__

namespace BW
{

	class TerrainManager
	{
		Urho3D::SharedPtr<Urho3D::Scene> m_spMainScene;
	public:
		TerrainManager(Urho3D::Context* pContext, Urho3D::SharedPtr<Urho3D::Scene> spMainScene);
		~TerrainManager();
		
		
		
	};

}

#endif // __BARRELWORLD_TERRAIN_MANAGER__H__
