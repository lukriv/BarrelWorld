#include "TerrainTile.h"
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

static const float VERTICAL_SPACING = 1.0f;
static const float HORIZONTAL_SPACING = 2.0f;

static const int32_t GRASS_TO_GROW_TEMPERATURE = 5;
static const int32_t ALTITUDE_CHANGE_TO_TEMPERATURE = 6;

static const uint8_t SEA_LEVEL_ALTITUDE = 15; 



//static const char* TERRAIN_WEIGHTS_RESOURCE = "Manual/TerrainWeights.dds";

BW::TerrainTile::TerrainTile(Urho3D::Application *pApp, Urho3D::Scene *pMainScene) : 
	m_pApp(pApp), m_spMainScene(pMainScene)
{
}

BW::TerrainTile::~TerrainTile()
{
	
}

void BW::TerrainTile::GenerateTerrainParameters(TerrainParams& params)
{
	HillParams hillParams;
	//set parameters
	
	hillParams.m_count = 1;
	hillParams.m_maxHeight = 39;
	hillParams.m_minHeight = 15;
	hillParams.m_minTopSize = 3;
	hillParams.m_maxTopSize = 6;
	hillParams.m_maxDifference = 3;
	hillParams.m_dispersion = 6;
	hillParams.m_terrainSides = TERR_SIDE_NORTH | TERR_SIDE_WEST;
	
	params.m_hills.push_back(hillParams);
	
	hillParams.m_count = 15;
	hillParams.m_maxHeight = 12;
	hillParams.m_minHeight = 3;
	hillParams.m_minTopSize = 9;
	hillParams.m_maxTopSize = 15;
	hillParams.m_maxDifference = 1;
	hillParams.m_dispersion = 0;
	hillParams.m_terrainSides = TERR_SIDE_NORTH | TERR_SIDE_EAST;
	
	
	params.m_hills.push_back(hillParams);
	
	WeatherParams weather;
	weather.m_temperature = 6;
	weather.m_terrainSides = TERR_SIDE_UNDEFINED;
	
	params.m_weather.push_back(weather);
	
	GroundParams ground;
	ground.m_baseAlt = 15;
	ground.m_terrainSides = TERR_SIDE_UNDEFINED;
	
	params.m_ground.push_back(ground);
	
	ground.m_baseAlt = 18;
	ground.m_terrainSides = TERR_SIDE_UNDEFINED;
	
	params.m_ground.push_back(ground);

	
	params.m_baseAlt = 15;
	
	// default 
	params.m_defaultType = BW::TERR_ROCK;
	params.m_slope = 45;
	
	// snow
	params.m_terrainTexTable = MODERATE_BELT;
	
}

void BW::TerrainTile::GenerateTerrain()
{
	TerrainParams params;
	
	GenerateTerrainParameters(params);
	
	GenerateTerrainHeightAndMat(params);
	//Create heightmap terrain with collision
    m_spTerrainNode = m_spMainScene->CreateChild("Terrain");
    m_spTerrainNode->SetPosition(Vector3::ZERO);
    Terrain* terrain = m_spTerrainNode->CreateComponent<Terrain>();
    terrain->SetPatchSize(64);
    terrain->SetSpacing(Vector3(HORIZONTAL_SPACING, VERTICAL_SPACING, HORIZONTAL_SPACING)); // Spacing between vertices and vertical resolution of the height map
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



void BW::TerrainTile::GenerateTerrainHeightAndMat(const TerrainParams &params)
{
	
	ResetMapImages(params);
	
	unsigned char* data = m_spHeightMap->GetData();
	
	GenerateGrounds(m_spHeightMap->GetData(), CONST_MAP_SIZE, params);
	
	GenerateHills(data, CONST_MAP_SIZE, params);
	
	GenerateAltWeights(m_spWeightMap->GetData(), m_spWeightMap2->GetData(), CONST_WEIGHTS_SIZE, m_spHeightMap->GetData(), CONST_MAP_SIZE, params);
	
	//m_spHeightMap->SavePNG(String("test.png"));
	//m_spWeightMap->SavePNG(String("weights.png"));
	
	PrepareMapMaterial(params);
	
		
}

Urho3D::Node* BW::TerrainTile::GetTerrainNode()
{
	return m_spTerrainNode;
}

void BW::TerrainTile::GetTerrainCircle(const IntVector2& center, int32_t radius, std::vector<IntVector2> &output)
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

void BW::TerrainTile::GetTerrainLine(const Urho3D::IntVector2& p0, const Urho3D::IntVector2& p1, std::vector<Urho3D::IntVector2> &output)
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

void BW::TerrainTile::GetTerrainLine3d(const Urho3D::IntVector3& p0, const Urho3D::IntVector3& p1, std::vector<Urho3D::IntVector3>& output)
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

void BW::TerrainTile::GenerateHills(unsigned char* data, int32_t MAP_SIZE, const TerrainParams& params)
{ 
	static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	for(auto &hills: params.m_hills)
	{
		
		// hills
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
		
		std::uniform_int_distribution<int32_t> altDist(hills.m_minHeight/hills.m_maxDifference,hills.m_maxHeight/hills.m_maxDifference);
		std::uniform_int_distribution<int32_t> wideDistX( ((hills.m_terrainSides & TERR_SIDE_WEST) != 0) ? 0 : CONST_MAP_SIZE / 2
															,((hills.m_terrainSides & TERR_SIDE_EAST) != 0) ? (CONST_MAP_SIZE - 1) : CONST_MAP_SIZE / 2);
		std::uniform_int_distribution<int32_t> wideDistY( ((hills.m_terrainSides & TERR_SIDE_NORTH) != 0) ? 0 : CONST_MAP_SIZE / 2
															,((hills.m_terrainSides & TERR_SIDE_SOUTH) != 0) ? (CONST_MAP_SIZE - 1) : CONST_MAP_SIZE / 2);
		std::uniform_int_distribution<int32_t> topDist(-hills.m_dispersion, hills.m_dispersion);
		std::uniform_int_distribution<int32_t> topSize(hills.m_minTopSize, hills.m_maxTopSize);

		// generate hills
		for (int32_t i = 0; i < hills.m_count; ++i)
		{
			myQueue.push(StackItem(altDist(generator) * hills.m_maxDifference, IntVector2(wideDistX(generator), wideDistY(generator))));
		}
		
		std::vector<IntVector3> midpointLine;
		//std::vector<IntVector2> v1;
		
		//unsigned char* point = nullptr;
		
		while(!myQueue.empty())
		{
			const StackItem &item = myQueue.top();
			
			uint8_t alt = item.m_alt + params.m_baseAlt;
			
			// skip hills with min altitude
			if(alt <= params.m_baseAlt)
			{
				myQueue.pop();
				continue;
			}
			
			IntVector2 downPoint(topDist(generator), topDist(generator));
			
			downPoint = item.m_position + downPoint;
			GetTerrainLine3d(IntVector3(item.m_position.x_, item.m_position.y_, params.m_baseAlt/hills.m_maxDifference), 
					IntVector3(downPoint.x_, downPoint.y_, alt/hills.m_maxDifference), midpointLine);
			
			int8_t radius = topSize(generator);
			
			int32_t last_alt = 255;
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
				FillCircle(IntVector2(mPoint.x_, mPoint.y_), radius, alt, data, MAP_SIZE);
				
				//v1.clear();
				alt -= hills.m_maxDifference;
				radius += hills.m_maxDifference;
			
			}
			
			midpointLine.clear();
			myQueue.pop();
			
		}
	}
			
}


bool BW::TerrainTile::AddTerrainType(unsigned char* wBasePoint, unsigned char* wBasePoint2, ETerrainType terrType, const TerrainIndexTableRow* table, uint8_t weight)
{
	static const int32_t SHADER_COLOR = 3;
	
	unsigned char* base = nullptr;
	int32_t offset = 0;
	const TerrainIndexTableRow *pRow = table;
	while((terrType != pRow->m_texType)&&(pRow->m_texType != TERR_UNDEFINED))
	{
		++pRow;
	}
	
	if(pRow->m_texType == TERR_UNDEFINED)
	{
		return false;
	}
	
	if(pRow->m_index < SHADER_COLOR)
	{
		base = wBasePoint;
	} else {
		base = wBasePoint2;
	}
	
	offset = pRow->m_index % SHADER_COLOR;
	
	*(base + offset) = weight;
	
	return true;
}



void BW::TerrainTile::GenerateAltWeights(unsigned char* weightData, unsigned char* weightData2, int32_t WEIGHTS_SIZE, const unsigned char* data, int32_t MAP_SIZE, const TerrainParams& params)
{
	
	unsigned char* wBasePoint = nullptr;
	unsigned char* wBasePoint2 = nullptr;
	unsigned char altitude = 0;
	
	uint8_t corners[4];
	Vector2 normPos;
	float slope = 0.0;
	bool colorAdded;
	for(int32_t y = 0; y < WEIGHTS_SIZE; ++y)
	{
		for(int32_t x = 0; x < WEIGHTS_SIZE; ++x)
		{
			colorAdded = false;
			
			int32_t mx = (x*(MAP_SIZE-1)) / WEIGHTS_SIZE;
			int32_t my = (y*(MAP_SIZE-1)) / WEIGHTS_SIZE;
			
			wBasePoint = weightData + y*WEIGHTS_SIZE*WEIGHTS_COMPONENT + x*WEIGHTS_COMPONENT;
			wBasePoint2 = weightData2 + y*WEIGHTS_SIZE*WEIGHTS_COMPONENT + x*WEIGHTS_COMPONENT;
			
			corners[0] = *(data + my*MAP_SIZE + mx);
			corners[1] = *(data + my*MAP_SIZE + std::min(mx + 1, MAP_SIZE - 1));
			corners[2] = *(data + std::min(my + 1, MAP_SIZE - 1)*MAP_SIZE + mx);
			corners[3] = *(data + std::min(my + 1, MAP_SIZE - 1)*MAP_SIZE + std::min(mx + 1, MAP_SIZE - 1));
			
			normPos.x_ = (static_cast<float>(x) * static_cast<float>(MAP_SIZE)) / static_cast<float>(WEIGHTS_SIZE) - static_cast<float>(mx);
			normPos.y_ = (static_cast<float>(y) * static_cast<float>(MAP_SIZE)) / static_cast<float>(WEIGHTS_SIZE) - static_cast<float>(my);
			
			altitude = *(data + my*MAP_SIZE + mx);
			
			slope = GetSlopeAngle(normPos, corners);
			
			if(slope > params.m_slope)
			{
				colorAdded |= AddTerrainType(wBasePoint, wBasePoint2, params.m_defaultType, params.m_terrainTexTable, 255);
			} else {
				int32_t temperature = GetTemperature(altitude,GetWeatherParams(IntVector2(mx,my), params));
				if (temperature >= GRASS_TO_GROW_TEMPERATURE)
				{
					colorAdded |= AddTerrainType(wBasePoint, wBasePoint2, TERR_GRASS, params.m_terrainTexTable, 255);
				}
				
				if (temperature <= 0)
				{
					colorAdded |= AddTerrainType(wBasePoint, wBasePoint2, TERR_SNOW, params.m_terrainTexTable, 255);
				}
				
				if(!colorAdded)
				{
					colorAdded |= AddTerrainType(wBasePoint, wBasePoint2, params.m_defaultType, params.m_terrainTexTable, 255);
				}
			}
			
			if(!colorAdded)
			{
				String str;
				str.AppendWithFormat("[%d,%d], slope: %f, alt: %d, terrPos: [%d,%d]",x,y,slope,altitude, mx,my);
				Log::Write(LOG_WARNING, str);
			}
			
		}
	}
}



void BW::TerrainTile::GenerateGrounds(unsigned char* data, int32_t MAP_SIZE, const TerrainParams& params)
{
	//NW,NE,SW,SE
	static const int32_t NW=0,NE=1,SW=2,SE=3; // indexes
	int32_t corners[4] = {0,0,0,0};
	int32_t center = 0;
	for(auto &gParams : params.m_ground)
	{
		if(gParams.m_terrainSides == (TERR_SIDE_NORTH | TERR_SIDE_WEST))
		{
			corners[NW] = gParams.m_baseAlt;
			continue;
		}
		
		if(gParams.m_terrainSides == (TERR_SIDE_NORTH | TERR_SIDE_EAST))
		{
			corners[NE] = gParams.m_baseAlt;
			continue;
		}
		
		if(gParams.m_terrainSides == (TERR_SIDE_SOUTH | TERR_SIDE_WEST))
		{
			corners[SW] = gParams.m_baseAlt;
			continue;
		}
		
		if(gParams.m_terrainSides == (TERR_SIDE_SOUTH | TERR_SIDE_EAST))
		{
			corners[SE] = gParams.m_baseAlt;
			continue;
		}
		
		if(gParams.m_terrainSides == TERR_SIDE_NORTH)
		{
			corners[NW] = corners[NE] = gParams.m_baseAlt;
			continue;
		}

		if(gParams.m_terrainSides == TERR_SIDE_SOUTH)
		{
			corners[SW] = corners[SE] = gParams.m_baseAlt;
			continue;
		}
		
		if(gParams.m_terrainSides == TERR_SIDE_WEST)
		{
			corners[NW] = corners[SW] = gParams.m_baseAlt;
			continue;
		}
		
		if(gParams.m_terrainSides == TERR_SIDE_EAST)
		{
			corners[NE] = corners[SE] = gParams.m_baseAlt;
			continue;
		}
		
		corners[NW] = corners[NE] = corners[SW] = corners[SE] = gParams.m_baseAlt;
	}
	
	center = (corners[NW] + corners[NE] + corners[SW] + corners[SE]) / 4;
	
	if((corners[NW] == corners[NE]) && (corners[NE] == corners[SW]) && (corners[SW] == corners[SE]))
	{
		std::memset(data, (uint8_t)corners[NW], MAP_SIZE*MAP_SIZE);
		return;
	}
		
}

void BW::TerrainTile::ResetMapImages(const TerrainParams &params)
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
	std::memset( m_spHeightMap->GetData(), 0, CONST_MAP_SIZE*CONST_MAP_SIZE );
}

void BW::TerrainTile::PrepareMapMaterial(const TerrainParams& params)
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
	
	int tu_num = TU_SPECULAR;
	const TerrainIndexTableRow* pRow = params.m_terrainTexTable;

	while(pRow->m_texType != TERR_UNDEFINED)
	{
		m_spMaterial->SetTexture((TextureUnit)tu_num, (Texture*)cache->GetResource<Texture2D>(GetTextureName(pRow->m_texType)));
		++tu_num;
		++pRow;
	}
	
	m_spMaterial->SetShaderParameter ("MatSpecColor", Variant(Vector4(0.5, 0.5, 0.5, 16)));
	m_spMaterial->SetShaderParameter ("DetailTiling", Variant(Vector2(32, 32)));
}

float BW::TerrainTile::GetSlopeAngle(const Urho3D::Vector2& normPos, uint8_t cornersAltitude[4])
{
	if( (cornersAltitude[0] == cornersAltitude[1]) &&
		(cornersAltitude[1] == cornersAltitude[2]) &&
		(cornersAltitude[2] == cornersAltitude[3]))
	{
		return 0.0;
	}
	//String str;
	Vector3 normalVec;
	std::vector<int32_t> validIndex;
	
	if((cornersAltitude[0] == cornersAltitude[1]) && (cornersAltitude[2] == cornersAltitude[3]))
	{
		normalVec.x_ = 0.0;
		normalVec.y_ = HORIZONTAL_SPACING;
		normalVec.z_ = std::abs(static_cast<float>(cornersAltitude[2] - cornersAltitude[0]))*VERTICAL_SPACING;
		//str.Append("line y,");
	}
	
	if((cornersAltitude[0] == cornersAltitude[2]) && (cornersAltitude[1] == cornersAltitude[3]))
	{
		normalVec.x_ = std::abs(static_cast<float>(cornersAltitude[1] - cornersAltitude[0]))*VERTICAL_SPACING;
		normalVec.y_ = HORIZONTAL_SPACING;
		normalVec.z_ = 0.0;	
		//str.Append("line x,");
	}
	
	
	// diagonal check
	if(cornersAltitude[0] == cornersAltitude[3])
	{
		
		Vector3 v1,v2;
		if(normPos.x_ > normPos.y_)
		{
			v1 = Vector3(HORIZONTAL_SPACING, 0.0, HORIZONTAL_SPACING);
			v2 = Vector3(HORIZONTAL_SPACING, std::abs(static_cast<float>(cornersAltitude[1] - cornersAltitude[0])), 0.0);
			//str.Append("triangle 0-3-2,");
		} else {
			v1 = Vector3(0.0, std::abs(static_cast<float>(cornersAltitude[2] - cornersAltitude[0])), HORIZONTAL_SPACING);
			v2 = Vector3(HORIZONTAL_SPACING, 0.0, HORIZONTAL_SPACING);
			//str.Append("triangle 0-1-3,");
		}
		
		v1.Normalize();
		v2.Normalize();
			
		normalVec = v1.CrossProduct(v2);
	}
	
	if( cornersAltitude[1] == cornersAltitude[2])
	{
		Vector3 v1,v2;
		if((normPos.x_ + normPos.y_) < 1)
		{
			v1 = Vector3(-HORIZONTAL_SPACING, std::abs(static_cast<float>(cornersAltitude[0] - cornersAltitude[1])), 0.0);
			v2 = Vector3(-HORIZONTAL_SPACING, 0.0, HORIZONTAL_SPACING);
			//str.Append("triangle 1-0-2,");
		} else {
			v1 = Vector3(-HORIZONTAL_SPACING, 0.0, HORIZONTAL_SPACING);
			v2 = Vector3(0.0, std::abs(static_cast<float>(cornersAltitude[3] - cornersAltitude[1])), HORIZONTAL_SPACING);
			//str.Append("triangle 1-2-3,");
		}
		
		v1.Normalize();
		v2.Normalize();
			
		normalVec = v1.CrossProduct(v2);
	}
	
	// check diagonals
	normalVec.Normalize();
	//float angle = normalVec.Angle(Vector3(0.0, 1.0, 0.0));
	//
	//if(angle >0.1)
	//{
	//	str.AppendWithFormat("normal: %f %f %f, angle: %f", normalVec.x_, normalVec.y_, normalVec.z_, angle);
	//	Log::Write(LOG_INFO, str);
	//}
	
	return normalVec.Angle(Vector3(0.0, 1.0, 0.0));
}

float BW::TerrainTile::GetAltitude(const Urho3D::Vector2& normPos, uint8_t cornersAltitude[4])
{
	if( (cornersAltitude[0] == cornersAltitude[1]) &&
		(cornersAltitude[1] == cornersAltitude[2]) &&
		(cornersAltitude[2] == cornersAltitude[3]))
	{
		return static_cast<float>(cornersAltitude[0]);
	}
	
		
	
	return static_cast<float>(cornersAltitude[0]);
}

float BW::TerrainTile::GetLinearWeight(float pos, float begin, float end)
{
	if(begin > end)
	{
		std::swap(begin, end);
	}
	
	if (pos < begin)
	{
		return 0.0;
	}
	
	if (pos > end)
	{
		return 1.0;
	}
	
	return ((pos - begin) / (end - begin));
}

uint8_t BW::TerrainTile::GetLinearWeight255(float pos, float begin, float end)
{
	return static_cast<uint8_t>(GetLinearWeight(pos, begin, end)*255.0);
}



const BW::WeatherParams& BW::TerrainTile::GetWeatherParams(const Urho3D::IntVector2& pos, const TerrainParams& params)
{
	for(const BW::WeatherParams& wParams: params.m_weather)
	{
		if(((wParams.m_terrainSides & TERR_SIDE_NORTH) != 0) && (pos.y_ > (CONST_MAP_SIZE / 2)))
		{
			// we need north and position is south
			continue;
		}

		if(((wParams.m_terrainSides & TERR_SIDE_SOUTH) != 0) && (pos.y_ <= (CONST_MAP_SIZE / 2)))
		{
			// we need south and position is north
			continue;
		}
		
		if(((wParams.m_terrainSides & TERR_SIDE_WEST) != 0) && (pos.x_ > (CONST_MAP_SIZE / 2)))
		{
			// we need west and position is east
			continue;
		}

		if(((wParams.m_terrainSides & TERR_SIDE_EAST) != 0) && (pos.x_ <= (CONST_MAP_SIZE / 2)))
		{
			// we need east and position is west
			continue;
		}

		// probably we get acceptable weather parameters
		return wParams;
	}
	
	return params.m_weather[0]; // return first weather parameter
	
}

int32_t BW::TerrainTile::GetTemperature(uint8_t altitude, const WeatherParams& weatherParams)
{
	return (weatherParams.m_temperature - (altitude - SEA_LEVEL_ALTITUDE)/ALTITUDE_CHANGE_TO_TEMPERATURE);
}

const char* BW::TerrainTile::GetTextureName(ETerrainType terrType)
{
	const TerrainTexTableRow* pRow = BW::GLOBAL_TERRAIN_TABLE;
	while (pRow->m_texType != TERR_UNDEFINED)
	{
		if(pRow->m_texType == terrType)
		{
			return pRow->m_pTextureName;
		}
		
		++pRow;
	}
	
	return nullptr;
}

void BW::TerrainTile::FillCircle(const Urho3D::IntVector2& center, int32_t radius, uint8_t altitude, unsigned char* data, int32_t MAP_SIZE)
{
	std::vector<IntVector2> v1;
	GetTerrainCircle(center, radius, v1);
	
	std::sort(v1.begin(), v1.end(), [](const IntVector2& lhs, const IntVector2 &rhs) { return (lhs.y_ < rhs.y_);});
	
	int32_t y = -1;
	uint32_t i = 0;
	while( i < v1.size())
	{
		y = v1[i].y_;

		if ((y < 0) || (y >= MAP_SIZE))
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
		
		
		//if((x1 < 0)||(x1 >= MAP_SIZE)||(x2 < 0)||(x2 >= MAP_SIZE)||(y < 0)||(y >= MAP_SIZE))
		//{
		//	String str;
		//	str.AppendWithFormat("Danger: x1 = %d, x2 = %d, y = %d", x1, x2, y);
		//	Log::Write(LOG_WARNING, str);
		//}
		
		for (auto iter = data + y*MAP_SIZE + x1; iter < data + ((y)*MAP_SIZE) + (x2); ++iter)
		{
			if(*iter < altitude)
			{
				(*iter) = altitude;
			}
		}
			
	}
}
