#ifndef __BARRELWORLD_TERRAIN_MANAGER__H__
#define __BARRELWORLD_TERRAIN_MANAGER__H__

#include <cstdint>
#include <Urho3D/Container/RefCounted.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Engine/Application.h>

// forward declaration
namespace Urho3D {
	class Image;
}

namespace BW
{
	
	struct TerrainParams {
		uint8_t m_maxAlt;
		uint8_t m_minAlt;
		
		uint8_t m_hills; // hills top count
		uint8_t m_maxDifference; //max difference between neighbouring point
	};
	
	class TerrainManager : public Urho3D::RefCounted
	{
		Urho3D::Application *m_pApp;
		Urho3D::SharedPtr<Urho3D::Scene> m_spMainScene;
		Urho3D::SharedPtr<Urho3D::Node> m_spTerrainNode;
	public:
		TerrainManager(Urho3D::Application *pApp, Urho3D::SharedPtr<Urho3D::Scene> spMainScene);
		~TerrainManager();
		
		void GenerateTerrain();
		
		Urho3D::Node * GetTerrainNode();
		
	private:
		void GenerateTerrainHeightAndMat(const TerrainParams &params, Urho3D::SharedPtr<Urho3D::Image> &spImage, Urho3D::SharedPtr<Urho3D::Material> &spMaterial);
		
	};

}

#endif // __BARRELWORLD_TERRAIN_MANAGER__H__
