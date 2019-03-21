#ifndef __BARREL_WORLD_GEOMETRY_UTILITIES__H__
#define __BARREL_WORLD_GEOMETRY_UTILITIES__H__

#include <vector>
#include <map>
#include <Urho3D/Math/Vector3.h>

namespace BW
{
	
	
	class GeometryUtils
	{
	public:
		static void GenerateIcosahedron(int recursions, std::vector<Urho3D::Vector3> &vertices, std::vector<Urho3D::IntVector3> &faces);
		
		static void PrepareIcosahedronGeometryData(int recursions, std::vector<float> &verticesData, std::vector<unsigned short> &indexData);
		
	private:
		static void InitAsIcosohedron(std::vector<Urho3D::Vector3> &vertices, std::vector<Urho3D::IntVector3> &indexes);
		static void Subdivide(int recursions, std::vector<Urho3D::Vector3> &vertices, std::vector<Urho3D::IntVector3> &faces);
		static int GetMidPointIndex(std::map<int32_t, int32_t> &cache, int indexA, int indexB, std::vector<Urho3D::Vector3> &vertices);
	};

}

#endif // __BARREL_WORLD_GEOMETRY_UTILITIES__H__
