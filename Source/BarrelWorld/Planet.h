#ifndef __BARREL_WORLD_PLANET__H__
#define __BARREL_WORLD_PLANET__H__

#include "Game.h"
#include <vector>
#include <map>

namespace Urho3D {
	class Node;
	class String;
	class Vector3;
}

namespace BW
{

	class Planet
	{
	public:
		static Urho3D::Node* CreatePyramid(const Urho3D::String &name, Game *pApp);
		static Urho3D::Node* CreatePlanet(const Urho3D::String &name, Game *pApp);
		
	private:

		static void GenerateTriangleMesh(std::vector<Urho3D::Vector3> &vertices, std::vector<Urho3D::IntVector3> &indexes, std::vector<float> &verticesData, std::vector<unsigned short> &indexData);
		static void ConvertToDodecahedronMesh(std::vector<Urho3D::Vector3> &vertices, std::vector<Urho3D::IntVector3> &indexes);

	};

}

#endif // __BARREL_WORLD_PLANET__H__
