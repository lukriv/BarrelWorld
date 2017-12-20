#include "TerrainManager.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>


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
	ResourceCache* cache=m_pApp->GetSubsystem<ResourceCache>();
	
	GenerateTerrainHeightAndMat(spHeightMap, spMaterial);
	//Create heightmap terrain with collision
    m_spTerrainNode = m_spMainScene->CreateChild("Terrain");
    m_spTerrainNode->SetPosition(Vector3::ZERO);
    Terrain* terrain = m_spTerrainNode->CreateComponent<Terrain>();
    terrain->SetPatchSize(64);
    terrain->SetSpacing(Vector3(2.0f, 1.0f, 2.0f)); // Spacing between vertices and vertical resolution of the height map
    terrain->SetSmoothing(false);
    if(!terrain->SetHeightMap(spHeightMap))
	{
		Log::Write(LOG_ERROR, "Load heightmap failed");
	}
    terrain->SetMaterial(cache->GetResource<Material>("Materials/TerrainX.xml"));
    // The terrain consists of large triangles, which fits well for occlusion rendering, as a hill can occlude all
    // terrain patches and other objects behind it
    terrain->SetOccluder(false);
	
	// physics
	RigidBody* pBody = m_spTerrainNode->CreateComponent<RigidBody>();
    pBody->SetCollisionLayer(2); // Use layer bitmask 2 for static geometry
    CollisionShape* pShape = m_spTerrainNode->CreateComponent<CollisionShape>();
    pShape->SetTerrain();
	
}

void BW::TerrainManager::GenerateTerrainHeightAndMat(Urho3D::SharedPtr<Urho3D::Image>& spHeightMap, Urho3D::SharedPtr<Urho3D::Material>& spMaterial)
{
	//std::default_random_engine generator;
	//std::uniform_int_distribution<int> distribution(0,255);

	//auto dice = std::bind ( distribution, generator );
	//int wisdom = dice()+dice()+dice();
	
	//Create Image as height map
	spHeightMap = new Image(m_pApp->GetContext());
	
	spHeightMap->SetSize(257,257,1);
	
	std::memset( spHeightMap->GetData(), 1, 257*257 );
	
	std::memset( spHeightMap->GetData() + 126*257, 0, 257+257 );
	std::memset( spHeightMap->GetData() + 129*257, 2, 3*257 );	
	
	spHeightMap->SaveBMP(String("test.bmp"));
	
	
	//prepare Material
	spMaterial = new Material(m_pApp->GetContext());
	
}

Urho3D::Node* BW::TerrainManager::GetTerrainNode()
{
	return m_spTerrainNode;
}
