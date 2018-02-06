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
	params.m_hills = 10;
	params.m_maxAlt = 30;
	params.m_maxDifference = 3;
	params.m_minAlt = 3;
	
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
	std::uniform_int_distribution<int8_t> altDist(params.m_minAlt,params.m_maxAlt/params.m_maxDifference);
	std::uniform_int_distribution<int32_t> wideDist(0,255);
	std::uniform_int_distribution<int32_t> topDist(-params.m_maxDifference, params.m_maxDifference);

	auto altGen = std::bind(altDist, generator);
	auto wideGen = std::bind(wideDist, generator);
	auto topGen = std::bind(topDist, generator);
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
			myQueue.push(StackItem(altGen() * params.m_maxDifference, IntVector2(wideGen(), wideGen())));
		}
		
		std::vector<IntVector3> midpointLine;
		std::vector<IntVector2> v1;
		
		unsigned char* data = spHeightMap->GetData();
		//unsigned char* point = nullptr;
		
		while(!myQueue.empty())
		{
			const StackItem &item = myQueue.top();
			
			// skip hills with min altitude
			if(item.m_alt == params.m_minAlt)
			{
				myQueue.pop();
				continue;
			}
			
			IntVector2 downPoint(topGen(), topGen());
			
			downPoint = item.m_position + downPoint;
			GetTerrainLine3d(IntVector3(item.m_position.x_, item.m_position.y_, params.m_minAlt), 
					IntVector3(downPoint.x_, downPoint.y_, item.m_alt/params.m_maxDifference), midpointLine);
			
			int8_t radius = params.m_maxDifference;
			
			int32_t last_alt = params.m_maxAlt;
	
			Log::Write(LOG_INFO, "New hill");
			for (auto &mPoint : midpointLine)
			{
				String str;
				
				str.AppendWithFormat("Midpoint: %d, %d, %d", mPoint.x_, mPoint.y_, mPoint.z_);
				
				
				if(mPoint.z_ == last_alt)
				{
					str.Append(" skipped");
					Log::Write(LOG_INFO, str);
					continue;
				} else {
					last_alt = mPoint.z_;
				}
				
				Log::Write(LOG_INFO, str);
				
				GetTerrainCircle(IntVector2(mPoint.x_, mPoint.y_), radius, v1);
				
				std::sort(v1.begin(), v1.end(), [](const IntVector2& lhs, const IntVector2 &rhs) { return (lhs.y_ < rhs.y_);});
				
				int32_t y = -1;
				uint32_t i = 0;
				while( i < v1.size())
				{
					y = v1[i].y_;
					
					int32_t x1 = MAP_SIZE - 1;
					int32_t x2 = 0;					
					
					// find min max
					while((y == v1[i].y_)&&( i < v1.size()))
					{
						if(x1 > v1[i].x_) x1 = v1[i].x_;
						if(x2 < v1[i].x_) x2 = v1[i].x_;
						++i;
					}
					
					x1 = (x1 >= 0)? x1 : 0;
					x2 = (x2 < MAP_SIZE) ? x2 : MAP_SIZE - 1;
					
					for (auto iter = data + y*MAP_SIZE + x1; iter < data + ((y)*MAP_SIZE) + (x2); ++iter)
					{
						if(*iter == params.m_minAlt)
						{
							(*iter) = item.m_alt - radius;
						}
					}
						
				}
				
				v1.clear();
				
				radius += params.m_maxDifference;
			
			}
			
			midpointLine.clear();
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

void BW::TerrainManager::GetTerrainCircle(const IntVector2& center, int32_t radius, std::vector<IntVector2> &output)
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
		output.push_back(IntVector2(x0 - x, y0 + y));
		output.push_back(IntVector2(x0 + x, y0 + y));
		output.push_back(IntVector2(x0 - y, y0 + x));
		output.push_back(IntVector2(x0 + y, y0 + x));
		output.push_back(IntVector2(x0 - x, y0 - y));
		output.push_back(IntVector2(x0 + x, y0 - y));
		output.push_back(IntVector2(x0 - y, y0 - x));
		output.push_back(IntVector2(x0 + y, y0 - x));
		
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

void BW::TerrainManager::GetTerrainLine(const Urho3D::IntVector2& p0, const Urho3D::IntVector2& p1, std::vector<Urho3D::IntVector2> &output)
{
	int32_t x0 = p0.x_, y0 = p0.y_, x1 = p1.x_, y1 = p1.y_;
	int32_t dx = std::abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int32_t dy = std::abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int32_t err = (dx>dy ? dx : -dy)/2, e2;
 
	for(;;)
	{
		output.push_back(IntVector2(x0,y0));
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void BW::TerrainManager::GetTerrainLine3d(const Urho3D::IntVector3& p0, const Urho3D::IntVector3& p1, std::vector<Urho3D::IntVector3>& output)
{
	int32_t x0 = p0.x_, y0 = p0.y_, z0 = p0.z_, x1 = p1.x_, y1 = p1.y_, z1 = p1.z_;
	int32_t dx = std::abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int32_t dy = std::abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int32_t dz = std::abs(z1-z0), sz = z0<z1 ? 1 : -1; 
	int32_t dm = std::max(dx,std::max(dy,dz)), i = dm; /* maximum difference */
	x1 = y1 = z1 = dm/2; /* error offset */
	
	for(;;) {  /* loop */
		//setPixel(x0,y0,z0);
		output.push_back(IntVector3(x0, y0, z0));
		if (i-- == 0) break;
		x1 -= dx; if (x1 < 0) { x1 += dm; x0 += sx; } 
		y1 -= dy; if (y1 < 0) { y1 += dm; y0 += sy; } 
		z1 -= dz; if (z1 < 0) { z1 += dm; z0 += sz; } 
	}
}
