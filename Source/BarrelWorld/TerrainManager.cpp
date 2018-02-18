#include "TerrainManager.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/Graphics.h>


#include "LayerDefs.h"
#include "DebugTools.h"

#ifdef max
#undef max
#endif

#include <random>
#include <functional>
#include <algorithm>
#include <queue>
#include <chrono>

using namespace Urho3D;

#define ENABLE_LOGS 1

static const int32_t CONST_WEIGHTS_SIZE = 1024;
static const int32_t WEIGHTS_COMPONENT = 4;

static const int32_t CONST_MAP_SIZE = 257;

//terrain texture table row
struct TerrainTexTableRow {
	BW::ETerrainType m_texType;
	const char * m_pTextureName;
};

// standard texture table
TerrainTexTableRow MODERATE_BELT[] = {
	{BW::TERR_GRASS, "Textures/webGrass.dds"},
	{BW::TERR_ROCK, "Textures/webStone.dds"	},
	{BW::TERR_SNOW, "Textures/webSnow.dds"	},
	{BW::TERR_SAND, "Textures/webSand.dds" 	},
	{BW::TERR_DIRT, "Textures/webDirt.dds"	},
	{BW::TERR_BLACK, "Textures/black.dds"	}
	
};

//static const char* TERRAIN_WEIGHTS_RESOURCE = "Manual/TerrainWeights.dds";

BW::TerrainManager::TerrainManager(Urho3D::Application *pApp, Urho3D::Scene *pMainScene) : 
	m_pApp(pApp), m_spMainScene(pMainScene)
{
}

BW::TerrainManager::~TerrainManager()
{
	
}


void BW::TerrainManager::GenerateTerrain()
{
	TerrainParams params;
	//set parameters
	params.m_hills = 10;
	params.m_maxAlt = 30;
	params.m_maxDifference = 3;
	params.m_minAlt = 3;
	
	params.m_snowAlt = 17;
	params.m_rockAlt = 9;
	
	GenerateTerrainHeightAndMat(params);
	//Create heightmap terrain with collision
    m_spTerrainNode = m_spMainScene->CreateChild("Terrain");
    m_spTerrainNode->SetPosition(Vector3::ZERO);
    Terrain* terrain = m_spTerrainNode->CreateComponent<Terrain>();
    terrain->SetPatchSize(64);
    terrain->SetSpacing(Vector3(2.0f, 1.0f, 2.0f)); // Spacing between vertices and vertical resolution of the height map
    terrain->SetSmoothing(false);
    if(!terrain->SetHeightMap(m_spHeightMap))
	{
		Log::Write(LOG_ERROR, "Load heightmap failed");
	}
    terrain->SetMaterial(m_spMaterial);
    // The terrain consists of large triangles, which fits well for occlusion rendering, as a hill can occlude all
    // terrain patches and other objects behind it
    terrain->SetOccluder(false);
	
	// physics
	RigidBody* pBody = m_spTerrainNode->CreateComponent<RigidBody>();
    pBody->SetCollisionLayer(BW::LAYER_TERRAIN); // Use layer bitmask 2 for static geometry
    CollisionShape* pShape = m_spTerrainNode->CreateComponent<CollisionShape>();
    pShape->SetTerrain();
	
}



void BW::TerrainManager::GenerateTerrainHeightAndMat(const TerrainParams &params)
{
	//auto dice = std::bind ( distribution, generator );
	//int wisdom = dice()+dice()+dice();
	
	ResetMapImages(params);
	
	unsigned char* data = m_spHeightMap->GetData();
	
	GenerateHills(data, CONST_MAP_SIZE, params);
	
	GenerateAltWeights(m_spWeightMap->GetData(), m_spWeightMap2->GetData(), CONST_WEIGHTS_SIZE, m_spHeightMap->GetData(), CONST_MAP_SIZE, params);
	
	//m_spHeightMap->SavePNG(String("test.png"));
	//m_spWeightMap->SavePNG(String("weights.png"));
	
	PrepareMapMaterial(params);
	
		
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

void BW::TerrainManager::GenerateHills(unsigned char* data, int32_t MAP_SIZE, const TerrainParams& params)
{ // hills
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
	
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int32_t> altDist(params.m_minAlt,params.m_maxAlt/params.m_maxDifference);
	std::uniform_int_distribution<int32_t> wideDist(0,255);
	std::uniform_int_distribution<int32_t> topDist(-params.m_maxDifference, params.m_maxDifference);

	auto altGen = std::bind(altDist, generator);
	auto wideGen = std::bind(wideDist, generator);
	auto topGen = std::bind(topDist, generator);

	// generate hills
	for (int32_t i = 0; i < params.m_hills; ++i)
	{
		myQueue.push(StackItem(altGen() * params.m_maxDifference, IntVector2(wideGen(), wideGen())));
	}
	
	std::vector<IntVector3> midpointLine;
	std::vector<IntVector2> v1;
	
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
#ifdef ENABLE_LOGS
		Log::Write(LOG_INFO, "New hill");
#endif
		for (auto &mPoint : midpointLine)
		{
			String str;
#ifdef ENABLE_LOGS
			str.AppendWithFormat("Midpoint: %d, %d, %d", mPoint.x_, mPoint.y_, mPoint.z_);
#endif
			
			if(mPoint.z_ == last_alt)
			{
#ifdef ENABLE_LOGS
				str.Append(" skipped");
				Log::Write(LOG_INFO, str);
#endif
				continue;
			} else {
				last_alt = mPoint.z_;
			}
#ifdef ENABLE_LOGS			
			Log::Write(LOG_INFO, str);
#endif
			
			GetTerrainCircle(IntVector2(mPoint.x_, mPoint.y_), radius, v1);
			
			std::sort(v1.begin(), v1.end(), [](const IntVector2& lhs, const IntVector2 &rhs) { return (lhs.y_ < rhs.y_);});
			
			int32_t y = -1;
			uint32_t i = 0;
			while( i < v1.size())
			{
				y = v1[i].y_;

				if ((y < 0) || (y > MAP_SIZE))
				{
					++i;
					continue;
				}
				
				int32_t x1 = MAP_SIZE - 1;
				int32_t x2 = 0;					
				
				// find min max
				while((i < v1.size())&&(y == v1[i].y_))
				{
					if(x1 > v1[i].x_) x1 = v1[i].x_;
					if(x2 < v1[i].x_) x2 = v1[i].x_;
					++i;
				}
				
				x1 = (x1 >= 0)? x1 : 0;
				x2 = (x2 < MAP_SIZE) ? x2 : MAP_SIZE - 1;
				
				for (auto iter = data + y*MAP_SIZE + x1; iter < data + ((y)*MAP_SIZE) + (x2); ++iter)
				{
					if(*iter < (item.m_alt - radius))
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

void BW::TerrainManager::GenerateAltWeights(unsigned char* weightData, unsigned char* weightData2, int32_t WEIGHTS_SIZE, const unsigned char* data, int32_t MAP_SIZE, const TerrainParams& params)
{
	

	
	ETerrainType terrTex = TERR_GRASS;
	
	unsigned char* wBasePoint = nullptr;
	unsigned char* wBasePoint2 = nullptr;
	unsigned char altitude = 0;
	
	float corners[4];
	IntVector2 normPos;
	for(int32_t y = 0; y < WEIGHTS_SIZE; ++y)
	{
		for(int32_t x = 0; x < WEIGHTS_SIZE; ++x)
		{
			int32_t mx = (x*MAP_SIZE) / WEIGHTS_SIZE;
			int32_t my = (y*MAP_SIZE) / WEIGHTS_SIZE;
			
			wBasePoint = weightData + y*WEIGHTS_SIZE*WEIGHTS_COMPONENT + x*WEIGHTS_COMPONENT;
			wBasePoint2 = weightData2 + y*WEIGHTS_SIZE*WEIGHTS_COMPONENT + x*WEIGHTS_COMPONENT;
			
			corners[0] = *(data + my*MAP_SIZE + mx);
			corners[1] = *(data + my*MAP_SIZE + std::min(mx + 1, MAP_SIZE - 1));
			corners[2] = *(data + std::min(my + 1, MAP_SIZE - 1)*MAP_SIZE + mx);
			corners[3] = *(data + std::min(my + 1, MAP_SIZE - 1)*MAP_SIZE + std::min(mx + 1, MAP_SIZE - 1));
			
			normPos.x_ = (static_cast<float>(x) * static_cast<float>(MAP_SIZE)) / static_cast<float>(WEIGHTS_SIZE) - static_cast<float>(mx);
			normPos.y_ = (static_cast<float>(y) * static_cast<float>(MAP_SIZE)) / static_cast<float>(WEIGHTS_SIZE) - static_cast<float>(my);
			
			altitude = *(data + my*MAP_SIZE + mx);
			
			if( altitude > params.m_snowAlt )
			{
				terrTex = TERR_SNOW;
			} else if ( altitude > params.m_rockAlt ) {
				terrTex = TERR_ROCK;
			} else if ( altitude > 4 ){
				terrTex = TERR_DIRT;
			} else {
				terrTex = TERR_GRASS;
			}
			
			switch(terrTex)
			{
				case TERR_GRASS:
					*(wBasePoint + 1) = 255; //red
					break;
				case TERR_ROCK :
					*(wBasePoint + 2) = 255; // green
					break;
				case TERR_SNOW :
					*(wBasePoint2) = 255;
					break;
				case TERR_SAND :
					*(wBasePoint) = 255; //red
					break;
				case TERR_DIRT :
					*(wBasePoint2 + 1) = 255; // blue
					break;
				default:
					break;
			}
			
		}
	}
}

void BW::TerrainManager::ResetMapImages(const TerrainParams &params)
{
	//Create Image as height map
	if(m_spWeightMap.Null())
	{
		m_spWeightMap = new Image(m_pApp->GetContext());
		m_spWeightMap->SetSize(CONST_WEIGHTS_SIZE, CONST_WEIGHTS_SIZE, WEIGHTS_COMPONENT);
	}
	
	if(m_spWeightMap2.Null())
	{
		m_spWeightMap2 = new Image(m_pApp->GetContext());
		m_spWeightMap2->SetSize(CONST_WEIGHTS_SIZE, CONST_WEIGHTS_SIZE, WEIGHTS_COMPONENT);
	}
	
	if(m_spHeightMap.Null())
	{
		m_spHeightMap = new Image(m_pApp->GetContext());
		m_spHeightMap->SetSize(CONST_MAP_SIZE,CONST_MAP_SIZE,1);
	}
	
	std::memset( m_spWeightMap->GetData(), 0, CONST_WEIGHTS_SIZE*CONST_WEIGHTS_SIZE*WEIGHTS_COMPONENT );
	std::memset( m_spWeightMap2->GetData(), 0, CONST_WEIGHTS_SIZE*CONST_WEIGHTS_SIZE*WEIGHTS_COMPONENT );
	std::memset( m_spHeightMap->GetData(), params.m_minAlt, CONST_MAP_SIZE*CONST_MAP_SIZE );
}

void BW::TerrainManager::PrepareMapMaterial(const TerrainParams& params)
{
	ResourceCache* cache=m_pApp->GetSubsystem<ResourceCache>();
	//prepare Material
	if(m_spMaterial.Null())
	{
		m_spMaterial = new Material(m_pApp->GetContext());
		m_spMaterial->SetTechnique(0, cache->GetResource<Technique>("Techniques/TerrainBlend6.xml"));
	}
	
	//m_spMaterial->SetTexture(TU_DIFFUSE, (Texture*)cache->GetResource<Texture2D>("Textures/TerrainWeights.dds"));
	if(m_spWeightTex.Null())
	{
		m_spWeightTex = new Texture2D(m_pApp->GetContext());
		m_spWeightTex->SetSize(CONST_WEIGHTS_SIZE, CONST_WEIGHTS_SIZE, Urho3D::Graphics::GetRGBAFormat(), TEXTURE_DYNAMIC);
	}
	
	if(m_spWeightTex2.Null())
	{
		m_spWeightTex2 = new Texture2D(m_pApp->GetContext());
		m_spWeightTex2->SetSize(CONST_WEIGHTS_SIZE, CONST_WEIGHTS_SIZE, Urho3D::Graphics::GetRGBAFormat(), TEXTURE_DYNAMIC);
	}

	m_spWeightTex->SetData(m_spWeightMap, false);
	m_spWeightTex2->SetData(m_spWeightMap2, false);
	
	
	
	m_spMaterial->SetTexture(TU_DIFFUSE, m_spWeightTex);
	m_spMaterial->SetTexture(TU_NORMAL, m_spWeightTex2);
	m_spMaterial->SetTexture(TU_SPECULAR, (Texture*)cache->GetResource<Texture2D>("Textures/webSand.dds"));
	m_spMaterial->SetTexture(TU_EMISSIVE, (Texture*)cache->GetResource<Texture2D>("Textures/webGrass.dds"));
	m_spMaterial->SetTexture(TU_ENVIRONMENT, (Texture*)cache->GetResource<Texture2D>("Textures/webStone.dds"));
	m_spMaterial->SetTexture(TU_VOLUMEMAP, (Texture*)cache->GetResource<Texture2D>("Textures/webSnow.dds"));
	m_spMaterial->SetTexture(TU_CUSTOM1, (Texture*)cache->GetResource<Texture2D>("Textures/webDirt.dds"));
	m_spMaterial->SetTexture(TU_CUSTOM2, (Texture*)cache->GetResource<Texture2D>("Textures/TerrainDetail1.dds"));
	
	m_spMaterial->SetShaderParameter ("MatSpecColor", Variant(Vector4(0.5, 0.5, 0.5, 16)));
	m_spMaterial->SetShaderParameter ("DetailTiling", Variant(Vector2(32, 32)));
}

float BW::TerrainManager::GetSlopeAngle(const Urho3D::IntVector2& normPos, float cornersAltitude[4])
{
}

float BW::TerrainManager::GetAltitude(const Urho3D::IntVector2& normPos, float cornersAltitude[4])
{
}
