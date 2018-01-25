#include "TerrainManager.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Texture2D.h>

#include "LayerDefs.h"
#include "DebugTools.h"

#include <random>
#include <functional>
#include <algorithm>
#include <queue>
#include <chrono>

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
	//ResourceCache* cache=m_pApp->GetSubsystem<ResourceCache>();
	TerrainParams params;
	//set parameters
	params.m_hills = 9;
	params.m_maxAlt = 20;
	params.m_maxDifference = 1;
	params.m_minAlt = 0;
	
	GenerateTerrainHeightAndMat(params, spHeightMap, spMaterial);
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
    terrain->SetMaterial(spMaterial);
    // The terrain consists of large triangles, which fits well for occlusion rendering, as a hill can occlude all
    // terrain patches and other objects behind it
    terrain->SetOccluder(false);
	
	// physics
	RigidBody* pBody = m_spTerrainNode->CreateComponent<RigidBody>();
    pBody->SetCollisionLayer(BW::LAYER_TERRAIN); // Use layer bitmask 2 for static geometry
    CollisionShape* pShape = m_spTerrainNode->CreateComponent<CollisionShape>();
    pShape->SetTerrain();
	
}



void BW::TerrainManager::GenerateTerrainHeightAndMat(const TerrainParams &params, Urho3D::SharedPtr<Urho3D::Image>& spHeightMap, Urho3D::SharedPtr<Urho3D::Material>& spMaterial)
{
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int8_t> altDist(params.m_minAlt,params.m_maxAlt);
	std::uniform_int_distribution<int32_t> wideDist(0,256);

	auto altGen = std::bind(altDist, generator);
	auto wideGen = std::bind(wideDist, generator);
	//auto dice = std::bind ( distribution, generator );
	//int wisdom = dice()+dice()+dice();
	
	ResourceCache* cache=m_pApp->GetSubsystem<ResourceCache>();
	
	//Create Image as height map
	spHeightMap = new Image(m_pApp->GetContext());
	
	const int32_t MAP_SIZE = 257;
	
	spHeightMap->SetSize(MAP_SIZE,MAP_SIZE,1);
	
	std::memset( spHeightMap->GetData(), params.m_minAlt, MAP_SIZE*MAP_SIZE );
	
	{
		struct StackItem {
			int8_t m_alt;
			IntVector2 m_position;
			
			StackItem(int8_t alt, const IntVector2 &pos) : m_alt(alt), m_position(pos) {}
		};

		struct mycomparison
		{
		  bool operator() (const StackItem& lhs, const StackItem&rhs) const
		  {
			return (lhs.m_alt < rhs.m_alt);
		  }
		};
		std::priority_queue<StackItem, std::vector<StackItem>, mycomparison > myQueue;

		// generate hills
		for (int32_t i = 0; i < params.m_hills; ++i)
		{
			myQueue.push(StackItem(altGen(), IntVector2(wideGen(), wideGen())));
		}
		
		unsigned char* data = spHeightMap->GetData();
		unsigned char* point = nullptr;
		
		while(!myQueue.empty())
		{
			const StackItem &item = myQueue.top();
			
			if((item.m_position.y_ >= MAP_SIZE)||
				(item.m_position.x_ >= MAP_SIZE)||
				(item.m_position.x_ < 0)||
				(item.m_position.x_ < 0))
			{
				myQueue.pop();
				continue;
			}
			
			
			point = data +  item.m_position.y_ * MAP_SIZE + item.m_position.x_;
			// set altitude
			if(( *(point) == params.m_minAlt) && (item.m_alt > params.m_minAlt))
			{
				*(point) = item.m_alt;
			} else {
				myQueue.pop();
				continue;
			}
			
			
			
			myQueue.push(StackItem(item.m_alt - 1, IntVector2(item.m_position.x_ + 1, item.m_position.y_ + 1)));
			myQueue.push(StackItem(item.m_alt - 1, IntVector2(item.m_position.x_, item.m_position.y_ + 1)));
			myQueue.push(StackItem(item.m_alt - 1, IntVector2(item.m_position.x_ - 1, item.m_position.y_ + 1)));
			myQueue.push(StackItem(item.m_alt - 1, IntVector2(item.m_position.x_ - 1, item.m_position.y_)));
			myQueue.push(StackItem(item.m_alt - 1, IntVector2(item.m_position.x_ - 1, item.m_position.y_ - 1)));
			myQueue.push(StackItem(item.m_alt - 1, IntVector2(item.m_position.x_, item.m_position.y_ - 1)));
			myQueue.push(StackItem(item.m_alt - 1, IntVector2(item.m_position.x_ + 1, item.m_position.y_ - 1)));
			myQueue.push(StackItem(item.m_alt - 1, IntVector2(item.m_position.x_ + 1, item.m_position.y_)));
			
			myQueue.pop();
			
		}
		
				
	}
	
	spHeightMap->SaveBMP(String("test.bmp"));
	
	//prepare Material
	spMaterial = new Material(m_pApp->GetContext());
	spMaterial->SetTechnique(0, cache->GetResource<Technique>("Techniques/TerrainBlend.xml"));
	
	spMaterial->SetTexture(TU_DIFFUSE, (Texture*)cache->GetResource<Texture2D>("Textures/TerrainWeights.dds"));
	spMaterial->SetTexture(TU_NORMAL, (Texture*)cache->GetResource<Texture2D>("Textures/webGrass.dds"));
	spMaterial->SetTexture(TU_SPECULAR, (Texture*)cache->GetResource<Texture2D>("Textures/stone.dds"));
	spMaterial->SetTexture(TU_EMISSIVE, (Texture*)cache->GetResource<Texture2D>("Textures/TerrainDetail3.dds"));
	
	spMaterial->SetShaderParameter ("MatSpecColor", Variant(Vector4(0.5, 0.5, 0.5, 16)));
	spMaterial->SetShaderParameter ("DetailTiling", Variant(Vector2(32, 32)));
	
		
}

Urho3D::Node* BW::TerrainManager::GetTerrainNode()
{
	return m_spTerrainNode;
}

void BW::TerrainManager::GetTerrainCircle(const IntVector2& center, int32_t radius, std::vector<IntVector2> &v1, std::vector<IntVector2> &v2)
{
	int32_t x0 = center.x_;
	int32_t y0 = center.y_;
    int32_t x = radius - 1;
    int32_t y = 0;
    int32_t dx = 1;
    int32_t dy = 1;
    int32_t err = dx - (radius << 1);

    while (x >= y)
    {
		v1.push_back(IntVector2(x0 - x, y0 + y));
		v2.push_back(IntVector2(x0 + x, y0 + y));
		v1.push_back(IntVector2(x0 - y, y0 + x));
		v2.push_back(IntVector2(x0 + y, y0 + x));
		v1.push_back(IntVector2(x0 - x, y0 - y));
		v2.push_back(IntVector2(x0 + x, y0 - y));
		v1.push_back(IntVector2(x0 - y, y0 - x));
		v2.push_back(IntVector2(x0 + y, y0 - x));
		
        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        else
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}
