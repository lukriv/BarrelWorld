#include "TerrainManager.h"
#include <Urho3D/Resource/Image.h>

using namespace Urho3D;

BW::TerrainManager::TerrainManager(Urho3D::Application *pApp, Urho3D::SharedPtr<Urho3D::Scene> spMainScene) : 
	m_pApp(pApp), m_spMainScene(spMainScene)
{
}

BW::TerrainManager::~TerrainManager()
{
}


void BW::TerrainManager::GenerateTerrain()
{
	Urho3D::SharedPtr<Urho3D::Image> spHeightMap;
	Urho3D::SharedPtr<Urho3D::Material> spMaterial;
	
	GenerateTerrainHeightAndMat(spHeightMap, spMaterial);
/*	// Create heightmap terrain with collision
    Node* terrainNode = scene_->CreateChild("Terrain");
    terrainNode->SetPosition(Vector3::ZERO);
    Terrain* terrain = terrainNode->CreateComponent<Terrain>();
    terrain->SetPatchSize(64);
    terrain->SetSpacing(Vector3(2.0f, 0.1f, 2.0f)); // Spacing between vertices and vertical resolution of the height map
    terrain->SetSmoothing(true);
    terrain->SetHeightMap(cache->GetResource<Image>("Textures/HeightMap.png"));
    terrain->SetMaterial(cache->GetResource<Material>("Materials/Terrain.xml"));
    // The terrain consists of large triangles, which fits well for occlusion rendering, as a hill can occlude all
    // terrain patches and other objects behind it
    terrain->SetOccluder(true);*/

	
}

void BW::TerrainManager::GenerateTerrainHeightAndMat(Urho3D::SharedPtr<Urho3D::Image>& spHeightMap, Urho3D::SharedPtr<Urho3D::Material>& spMaterial)
{
	//Create Image as height map
	spHeightMap = new Image(m_pApp->GetContext());
	
	spHeightMap->SetSize(32,32,1);
	
	spHeightMap->SaveBMP(String("test.bmp"));
	
	
	//prepare Material
	spMaterial = new Material(m_pApp->GetContext());
}
