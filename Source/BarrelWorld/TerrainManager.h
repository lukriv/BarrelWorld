#ifndef __BARRELWORLD_TERRAIN_MANAGER__H__
#define __BARRELWORLD_TERRAIN_MANAGER__H__

#include <cstdint>
#include <vector>
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
	
	enum ETerrainType {
		TERR_UNDEFINED = 0,
		TERR_GRASS = 1,
		TERR_ROCK = 2,
		TERR_SNOW = 3,
		TERR_SAND = 4,
		TERR_DIRT = 5,
		TERR_BLACK = 6
	};
	
	struct TerrainTypeAltitude {
		uint8_t m_altFrom;
		uint8_t m_altTo;
		ETerrainType m_type;
	};
	
	struct TerrainParams {
		uint8_t m_maxAlt;
		uint8_t m_minAlt;
		
		uint8_t m_hills; // hills top count
		uint8_t m_maxDifference; //max difference between neighbouring point
		
		uint8_t m_snowAlt;
		uint8_t m_rockAlt;
		
		std::vector<TerrainTypeAltitude> m_terrTypeList;
		
	};
	
	class TerrainManager : public Urho3D::RefCounted
	{
		Urho3D::Application *m_pApp;
		Urho3D::WeakPtr<Urho3D::Scene> m_spMainScene;
		Urho3D::WeakPtr<Urho3D::Node> m_spTerrainNode;
		
		Urho3D::SharedPtr<Urho3D::Image> m_spWeightMap;
		Urho3D::SharedPtr<Urho3D::Image> m_spWeightMap2;
		Urho3D::SharedPtr<Urho3D::Image> m_spHeightMap;
		
		Urho3D::SharedPtr<Urho3D::Material> m_spMaterial;

		Urho3D::SharedPtr<Urho3D::Texture2D> m_spWeightTex;
		Urho3D::SharedPtr<Urho3D::Texture2D> m_spWeightTex2;
	public:
		TerrainManager(Urho3D::Application *pApp, Urho3D::Scene *pMainScene);
		~TerrainManager();
		
		void GenerateTerrain();
		
		Urho3D::Node * GetTerrainNode();
		
	private:
		static void GetTerrainCircle(const Urho3D::IntVector2& center, int32_t radius, std::vector<Urho3D::IntVector2> &output);
		static void GetTerrainLine(const Urho3D::IntVector2& p0, const Urho3D::IntVector2& p1, std::vector<Urho3D::IntVector2> &output);
		static void GetTerrainLine3d(const Urho3D::IntVector3& p0, const Urho3D::IntVector3& p1, std::vector<Urho3D::IntVector3> &output);
		
		// compute slope angle
		// normPos - normalized position between 4 corners - x_ and y_ must be between 0 and 1
		// cornersAltitude altitude of four corners 
		// 0---1
		// |   |
		// 2---3
		// retval in degrees
		static float GetSlopeAngle(const Urho3D::IntVector2& normPos, float cornersAltitude[4]);
		
		// compute point altitude
		// normPos - normalized position between 4 corners - x_ and y_ must be between 0 and 1
		// cornersAltitude altitude of four corners 
		// 0---1
		// |   |
		// 2---3
		// retval altitude
		static float GetAltitude(const Urho3D::IntVector2& normPos, float cornersAltitude[4]);
		
		static void GenerateHills(unsigned char* data, int32_t MAX_SIZE, const TerrainParams& params);
		static void GenerateAltWeights(unsigned char* weightData, unsigned char* weightData2, int32_t WEIGHTS_SIZE, const unsigned char* data, int32_t MAP_SIZE, const TerrainParams& params);
		
		
		void ResetMapImages(const TerrainParams &params);
		void PrepareMapMaterial(const TerrainParams &params);
		
		void GenerateTerrainHeightAndMat(const TerrainParams &params);
		
	};

}

#endif // __BARRELWORLD_TERRAIN_MANAGER__H__
