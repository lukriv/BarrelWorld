#include "GeometryUtils.h"

using namespace Urho3D;

void BW::GeometryUtils::GenerateIcosahedron(int recursions, std::vector<Urho3D::Vector3>& vertices, std::vector<Urho3D::IntVector3>& faces)
{
	vertices.clear();
	faces.clear();
	InitAsIcosohedron(vertices, faces);
	Subdivide(recursions, vertices, faces);
}

void BW::GeometryUtils::InitAsIcosohedron(std::vector<Urho3D::Vector3>& vertices, std::vector<Urho3D::IntVector3>& indexes)
{
	float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

    vertices.push_back(Vector3(-1,  t,  0).Normalized()); //0
    vertices.push_back(Vector3( 1,  t,  0).Normalized()); //1
    vertices.push_back(Vector3(-1, -t,  0).Normalized()); //2
    vertices.push_back(Vector3( 1, -t,  0).Normalized()); //3
    vertices.push_back(Vector3( 0, -1,  t).Normalized()); //4
    vertices.push_back(Vector3( 0,  1,  t).Normalized()); //5
    vertices.push_back(Vector3( 0, -1, -t).Normalized()); //6
    vertices.push_back(Vector3( 0,  1, -t).Normalized()); //7
    vertices.push_back(Vector3( t,  0, -1).Normalized()); //8
    vertices.push_back(Vector3( t,  0,  1).Normalized()); //9
    vertices.push_back(Vector3(-t,  0, -1).Normalized()); //10
    vertices.push_back(Vector3(-t,  0,  1).Normalized()); //11

    // And here's the formula for the 20 sides,
    // referencing the 12 vertices we just created.

    indexes.push_back(IntVector3( 0, 11,  5));
    indexes.push_back(IntVector3( 0,  5,  1));
    indexes.push_back(IntVector3( 0,  1,  7));
    indexes.push_back(IntVector3( 0,  7, 10));
    indexes.push_back(IntVector3( 0, 10, 11));
    indexes.push_back(IntVector3( 1,  5,  9));
    indexes.push_back(IntVector3( 5, 11,  4));
    indexes.push_back(IntVector3(11, 10,  2));
    indexes.push_back(IntVector3(10,  7,  6));
    indexes.push_back(IntVector3( 7,  1,  8));
    indexes.push_back(IntVector3( 3,  9,  4));
    indexes.push_back(IntVector3( 3,  4,  2));
    indexes.push_back(IntVector3( 3,  2,  6));
    indexes.push_back(IntVector3( 3,  6,  8));
    indexes.push_back(IntVector3( 3,  8,  9));
    indexes.push_back(IntVector3( 4,  9,  5));
    indexes.push_back(IntVector3( 2,  4, 11));
    indexes.push_back(IntVector3( 6,  2, 10));
    indexes.push_back(IntVector3( 8,  6,  7));
    indexes.push_back(IntVector3( 9,  8,  1));
}

void BW::GeometryUtils::Subdivide(int recursions, std::vector<Urho3D::Vector3>& vertices, std::vector<Urho3D::IntVector3>& faces)
{
	std::map<int32_t, int32_t> midPointCache;

    for (int i = 0; i < recursions; i++)
    {
        std::vector<IntVector3> newFaces;
        for (auto& poly : faces)
        {
            int a = poly.x_;
            int b = poly.y_;
            int c = poly.z_;

            // Use GetMidPointIndex to either create a
            // new vertex between two old vertices, or
            // find the one that was already created.

            int ab = GetMidPointIndex(midPointCache, a, b, vertices);
            int bc = GetMidPointIndex(midPointCache, b, c, vertices);
            int ca = GetMidPointIndex(midPointCache, c, a, vertices);

            // Create the four new polygons using our original
            // three vertices, and the three new midpoints.
            newFaces.push_back(IntVector3(a, ab, ca));
            newFaces.push_back(IntVector3(b, bc, ab));
            newFaces.push_back(IntVector3(c, ca, bc));
            newFaces.push_back(IntVector3(ab, bc, ca));
        }
        // Replace all our old polygons with the new set of
        // subdivided ones.
        faces.swap(newFaces);
    }
}

int BW::GeometryUtils::GetMidPointIndex(std::map<int32_t, int32_t>& cache, int indexA, int indexB, std::vector<Urho3D::Vector3>& vertices)
{
	int smallerIndex = std::min(indexA, indexB);
    int greaterIndex = std::max(indexA, indexB);
    int key = (smallerIndex << 16) + greaterIndex;

    // If a midpoint is already defined, just return it.

    int ret;
	auto iter = cache.find(key);
	if (iter != cache.end()) return iter->second;

    // If we're here, it's because a midpoint for these two
    // vertices hasn't been created yet. Let's do that now!

    Vector3 p1 = vertices[indexA];
    Vector3 p2 = vertices[indexB];
    Vector3 middle = p1.Lerp(p2, 0.5f);
	middle.Normalize();

    ret = vertices.size();
    vertices.push_back(middle);

    // Add our new midpoint to the cache so we don't have
    // to do this again. =)

    cache.emplace(key, ret);
    return ret;
}

void BW::GeometryUtils::PrepareIcosahedronGeometryData(int recursions, std::vector<float>& verticesData, std::vector<unsigned short>& indexData)
{
	verticesData.clear();
	indexData.clear();
	
	std::vector<Vector3> vertices;
	std::vector<IntVector3> faces;
	
	InitAsIcosohedron(vertices, faces);
	Subdivide(recursions, vertices, faces);
	
	verticesData.reserve(vertices.size()*6);
	indexData.reserve(faces.size()*3);
	
	for(auto& ver: vertices)
	{
		Vector3 norm(ver.Normalized());
		verticesData.push_back(ver.x_);
		verticesData.push_back(ver.y_);
		verticesData.push_back(ver.z_);
		verticesData.push_back(norm.x_);
		verticesData.push_back(norm.y_);
		verticesData.push_back(norm.z_);
	}
	
	for(auto& face: faces)
	{
		indexData.push_back(face.x_);
		indexData.push_back(face.y_);
		indexData.push_back(face.z_);
	}
}
