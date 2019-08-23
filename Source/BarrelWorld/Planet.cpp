#include "Planet.h"


#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Graphics/IndexBuffer.h>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/IO/Log.h>
#include "Character.h"
#include "LayerDefs.h"
#include "DebugTools.h"

#include "GeometryUtils.h"

#include <set>

using namespace Urho3D;

/*
public class Planet 
{
    public Material m_Material;
    GameObject      m_PlanetMesh;

    List<Polygon> m_Polygons;
    List<Vector3> m_Vertices;

    public void Start()
    {
        InitAsIcosohedron();
        Subdivide(3);
        GenerateMesh();
    }

    public void InitAsIcosohedron()
    {
        m_Polygons = new List<Polygon>();
        m_Vertices = new List<Vector3>();

        // An icosahedron has 12 vertices, and
        // since they're completely symmetrical the
        // formula for calculating them is kind of
        // symmetrical too:

        float t = (1.0f + Mathf.Sqrt(5.0f)) / 2.0f;

        m_Vertices.Add(new Vector3(-1,  t,  0).normalized);
        m_Vertices.Add(new Vector3( 1,  t,  0).normalized);
        m_Vertices.Add(new Vector3(-1, -t,  0).normalized);
        m_Vertices.Add(new Vector3( 1, -t,  0).normalized);
        m_Vertices.Add(new Vector3( 0, -1,  t).normalized);
        m_Vertices.Add(new Vector3( 0,  1,  t).normalized);
        m_Vertices.Add(new Vector3( 0, -1, -t).normalized);
        m_Vertices.Add(new Vector3( 0,  1, -t).normalized);
        m_Vertices.Add(new Vector3( t,  0, -1).normalized);
        m_Vertices.Add(new Vector3( t,  0,  1).normalized);
        m_Vertices.Add(new Vector3(-t,  0, -1).normalized);
        m_Vertices.Add(new Vector3(-t,  0,  1).normalized);

        // And here's the formula for the 20 sides,
        // referencing the 12 vertices we just created.

        m_Polygons.Add(new Polygon( 0, 11,  5));
        m_Polygons.Add(new Polygon( 0,  5,  1));
        m_Polygons.Add(new Polygon( 0,  1,  7));
        m_Polygons.Add(new Polygon( 0,  7, 10));
        m_Polygons.Add(new Polygon( 0, 10, 11));
        m_Polygons.Add(new Polygon( 1,  5,  9));
        m_Polygons.Add(new Polygon( 5, 11,  4));
        m_Polygons.Add(new Polygon(11, 10,  2));
        m_Polygons.Add(new Polygon(10,  7,  6));
        m_Polygons.Add(new Polygon( 7,  1,  8));
        m_Polygons.Add(new Polygon( 3,  9,  4));
        m_Polygons.Add(new Polygon( 3,  4,  2));
        m_Polygons.Add(new Polygon( 3,  2,  6));
        m_Polygons.Add(new Polygon( 3,  6,  8));
        m_Polygons.Add(new Polygon( 3,  8,  9));
        m_Polygons.Add(new Polygon( 4,  9,  5));
        m_Polygons.Add(new Polygon( 2,  4, 11));
        m_Polygons.Add(new Polygon( 6,  2, 10));
        m_Polygons.Add(new Polygon( 8,  6,  7));
        m_Polygons.Add(new Polygon( 9,  8,  1));
    }

    public void Subdivide(int recursions)
    {
        var midPointCache = new Dictionary<int, int>();

        for (int i = 0; i < recursions; i++)
        {
            var newPolys = new List<Polygon>();
            foreach (var poly in m_Polygons)
            {
                int a = poly.m_Vertices[0];
                int b = poly.m_Vertices[1];
                int c = poly.m_Vertices[2];

                // Use GetMidPointIndex to either create a
                // new vertex between two old vertices, or
                // find the one that was already created.

                int ab = GetMidPointIndex(midPointCache, a, b);
                int bc = GetMidPointIndex(midPointCache, b, c);
                int ca = GetMidPointIndex(midPointCache, c, a);

                // Create the four new polygons using our original
                // three vertices, and the three new midpoints.
                newPolys.Add(new Polygon(a, ab, ca));
                newPolys.Add(new Polygon(b, bc, ab));
                newPolys.Add(new Polygon(c, ca, bc));
                newPolys.Add(new Polygon(ab, bc, ca));
            }
            // Replace all our old polygons with the new set of
            // subdivided ones.
            m_Polygons = newPolys;
        }
    }
    public int GetMidPointIndex(Dictionary<int, int> cache, int indexA, int indexB)
    {
        // We create a key out of the two original indices
        // by storing the smaller index in the upper two bytes
        // of an integer, and the larger index in the lower two
        // bytes. By sorting them according to whichever is smaller
        // we ensure that this function returns the same result
        // whether you call
        // GetMidPointIndex(cache, 5, 9)
        // or...
        // GetMidPointIndex(cache, 9, 5)

        int smallerIndex = Mathf.Min(indexA, indexB);
        int greaterIndex = Mathf.Max(indexA, indexB);
        int key = (smallerIndex << 16) + greaterIndex;

        // If a midpoint is already defined, just return it.

        int ret;
        if (cache.TryGetValue(key, out ret))
            return ret;

        // If we're here, it's because a midpoint for these two
        // vertices hasn't been created yet. Let's do that now!

        Vector3 p1 = m_Vertices[indexA];
        Vector3 p2 = m_Vertices[indexB];
        Vector3 middle = Vector3.Lerp(p1, p2, 0.5f).normalized;

        ret = m_Vertices.Count;
        m_Vertices.Add(middle);

        // Add our new midpoint to the cache so we don't have
        // to do this again. =)

        cache.Add(key, ret);
        return ret;
    }

    public void GenerateMesh()
    {
        if (m_PlanetMesh)
            Destroy(m_PlanetMesh);

        m_PlanetMesh = new GameObject("Planet Mesh");

        MeshRenderer surfaceRenderer = m_PlanetMesh.AddComponent<MeshRenderer>();
        surfaceRenderer.material     = m_Material;

        Mesh terrainMesh = new Mesh();

        int vertexCount = m_Polygons.Count * 3;

        int[] indices = new int[vertexCount];

        Vector3[] vertices = new Vector3[vertexCount];
        Vector3[] normals  = new Vector3[vertexCount];
        Color32[] colors   = new Color32[vertexCount];

        Color32 green = new Color32(20,  255, 30, 255);
        Color32 brown = new Color32(220, 150, 70, 255);

        for (int i = 0; i < m_Polygons.Count; i++)
        {
            var poly = m_Polygons[i];

            indices[i * 3 + 0] = i * 3 + 0;
            indices[i * 3 + 1] = i * 3 + 1;
            indices[i * 3 + 2] = i * 3 + 2;

            vertices[i * 3 + 0] = m_Vertices[poly.m_Vertices[0]];
            vertices[i * 3 + 1] = m_Vertices[poly.m_Vertices[1]];
            vertices[i * 3 + 2] = m_Vertices[poly.m_Vertices[2]];

            Color32 polyColor = Color32.Lerp(green, brown, Random.Range(0.0f, 1.0f)); 

            colors[i * 3 + 0] = polyColor;
            colors[i * 3 + 1] = polyColor;
            colors[i * 3 + 2] = polyColor;

            normals[i * 3 + 0] = m_Vertices[poly.m_Vertices[0]];
            normals[i * 3 + 1] = m_Vertices[poly.m_Vertices[1]];
            normals[i * 3 + 2] = m_Vertices[poly.m_Vertices[2]];
        }

        terrainMesh.vertices = vertices;
        terrainMesh.normals  = normals;
        terrainMesh.colors32 = colors;

        terrainMesh.SetTriangles(indices, 0);

        MeshFilter terrainFilter = m_PlanetMesh.AddComponent<MeshFilter>();
        terrainFilter.mesh = terrainMesh;
    }
}
*/

Urho3D::Node* BW::Planet::CreatePyramid(const Urho3D::String &name, Game *pApp)
{
	//* cache= pApp->GetSubsystem<ResourceCache>();
	Scene* pScene = pApp->GetMainScene();
	Node* pPlanetNode = pScene->CreateChild(name);
	
	
	// Finally create one model (pyramid shape) and a StaticModel to display it from scratch
    // Note: there are duplicated vertices to enable face normals. We will calculate normals programmatically
    {
        const unsigned numVertices = 18;
		auto context_ = pApp->GetContext();
        float vertexData[] = {
            // Position             Normal
            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,

            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,

            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,

            0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f,

            0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,

            0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 0.0f
        };

        const unsigned short indexData[] = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17
        };

        // Calculate face normals now
        for (unsigned i = 0; i < numVertices; i += 3)
        {
            Vector3& v1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i]));
            Vector3& v2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1)]));
            Vector3& v3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2)]));
            Vector3& n1 = *(reinterpret_cast<Vector3*>(&vertexData[6 * i + 3]));
            Vector3& n2 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 1) + 3]));
            Vector3& n3 = *(reinterpret_cast<Vector3*>(&vertexData[6 * (i + 2) + 3]));

            Vector3 edge1 = v1 - v2;
            Vector3 edge2 = v1 - v3;
            n1 = n2 = n3 = edge1.CrossProduct(edge2).Normalized();
        }

        SharedPtr<Model> fromScratchModel(new Model(context_));
        SharedPtr<VertexBuffer> vb(new VertexBuffer(context_));
        SharedPtr<IndexBuffer> ib(new IndexBuffer(context_));
        SharedPtr<Geometry> geom(new Geometry(context_));

        // Shadowed buffer needed for raycasts to work, and so that data can be automatically restored on device loss
        vb->SetShadowed(true);
        // We could use the "legacy" element bitmask to define elements for more compact code, but let's demonstrate
        // defining the vertex elements explicitly to allow any element types and order
        PODVector<VertexElement> elements;
        elements.Push(VertexElement(TYPE_VECTOR3, SEM_POSITION));
        elements.Push(VertexElement(TYPE_VECTOR3, SEM_NORMAL));
        vb->SetSize(numVertices, elements);
        vb->SetData(vertexData);

        ib->SetShadowed(true);
        ib->SetSize(numVertices, false);
        ib->SetData(indexData);

        geom->SetVertexBuffer(0, vb);
        geom->SetIndexBuffer(ib);
        geom->SetDrawRange(TRIANGLE_LIST, 0, numVertices);

        fromScratchModel->SetNumGeometries(1);
        fromScratchModel->SetGeometry(0, 0, geom);
        fromScratchModel->SetBoundingBox(BoundingBox(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f)));

        // Though not necessary to render, the vertex & index buffers must be listed in the model so that it can be saved properly
        Vector<SharedPtr<VertexBuffer> > vertexBuffers;
        Vector<SharedPtr<IndexBuffer> > indexBuffers;
        vertexBuffers.Push(vb);
        indexBuffers.Push(ib);
        // Morph ranges could also be not defined. Here we simply define a zero range (no morphing) for the vertex buffer
        PODVector<unsigned> morphRangeStarts;
        PODVector<unsigned> morphRangeCounts;
        morphRangeStarts.Push(0);
        morphRangeCounts.Push(0);
        fromScratchModel->SetVertexBuffers(vertexBuffers, morphRangeStarts, morphRangeCounts);
        fromScratchModel->SetIndexBuffers(indexBuffers);

        //Node* node = scene_->CreateChild("FromScratchObject");
        //node->SetPosition(Vector3(0.0f, 3.0f, 0.0f));
        StaticModel* object = pPlanetNode->CreateComponent<StaticModel>();
        object->SetModel(fromScratchModel);
    }
	
	return pPlanetNode;
}

void BW::Planet::GenerateTriangleMesh(std::vector<Vector3> &vertices, std::vector<IntVector3> &faces,	std::vector<float> &verticesData, std::vector<unsigned short> &indexData)
{
	//if(m_PlanetMesh)
    //Destroy(m_PlanetMesh);

    //m_PlanetMesh = new GameObject("Planet Mesh");

    //MeshRenderer surfaceRenderer = m_PlanetMesh.AddComponent<MeshRenderer>();
    //surfaceRenderer.material     = m_Material;

    //Mesh terrainMesh = new Mesh();

    int vertexCount = faces.size() * 3;

    indexData.resize(vertexCount);

	
    //Vector3[] vertices = new Vector3[vertexCount];
    //Vector3[] normals  = new Vector3[vertexCount];
    //Color32[] colors   = new Color32[vertexCount];
	verticesData.resize(vertexCount*6);

    //Color32 green = new Color32(20,  255, 30, 255);
    //Color32 brown = new Color32(220, 150, 70, 255);

    for (size_t i = 0; i < faces.size(); i++)
    {
        IntVector3& poly = faces[i];
		size_t vertIndex = i * 18;

        indexData[i * 3 + 0] = i * 3 + 0;
        indexData[i * 3 + 1] = i * 3 + 1;
        indexData[i * 3 + 2] = i * 3 + 2;
		//first
		verticesData[vertIndex + 0] = vertices[poly.x_].x_;
		verticesData[vertIndex + 1] = vertices[poly.x_].y_;
		verticesData[vertIndex + 2] = vertices[poly.x_].z_;
		
		verticesData[vertIndex + 3] = vertices[poly.x_].x_;
		verticesData[vertIndex + 4] = vertices[poly.x_].y_;
		verticesData[vertIndex + 5] = vertices[poly.x_].z_;
		//second
		verticesData[vertIndex + 6] = vertices[poly.y_].x_;
		verticesData[vertIndex + 7] = vertices[poly.y_].y_;
		verticesData[vertIndex + 8] = vertices[poly.y_].z_;
		
		verticesData[vertIndex + 9] = vertices[poly.y_].x_;
		verticesData[vertIndex + 10] = vertices[poly.y_].y_;
		verticesData[vertIndex + 11] = vertices[poly.y_].z_;
		//third
		verticesData[vertIndex + 12] = vertices[poly.z_].x_;
		verticesData[vertIndex + 13] = vertices[poly.z_].y_;
		verticesData[vertIndex + 14] = vertices[poly.z_].z_;
		
		verticesData[vertIndex + 15] = vertices[poly.z_].x_;
		verticesData[vertIndex + 16] = vertices[poly.z_].y_;
		verticesData[vertIndex + 17] = vertices[poly.z_].z_;
    }

}

Urho3D::Node* BW::Planet::CreatePlanet(const Urho3D::String& name, Game* pApp)
{
	std::vector<Vector3> vertices;
	std::vector<IntVector3> faces;
	
	std::vector<float> verticesData;
	std::vector<unsigned short> indexData;
	
	//InitAsIcosohedron(vertices, faces);
	//Subdivide(2, vertices, faces);
	
	BW::GeometryUtils::GenerateIcosahedron(1, vertices, faces);
	ConvertToDodecahedronMesh(vertices, faces);
	GenerateTriangleMesh(vertices, faces, verticesData, indexData);
	
	Scene* pScene = pApp->GetMainScene();
	Node* pPlanetNode = pScene->CreateChild(name);
	auto context_ = pApp->GetContext();
	
	SharedPtr<Model> fromScratchModel(new Model(context_));
    SharedPtr<VertexBuffer> vb(new VertexBuffer(context_));
    SharedPtr<IndexBuffer> ib(new IndexBuffer(context_));
    SharedPtr<Geometry> geom(new Geometry(context_));

    // Shadowed buffer needed for raycasts to work, and so that data can be automatically restored on device loss
    vb->SetShadowed(true);
    // We could use the "legacy" element bitmask to define elements for more compact code, but let's demonstrate
    // defining the vertex elements explicitly to allow any element types and order
    PODVector<VertexElement> elements;
    elements.Push(VertexElement(TYPE_VECTOR3, SEM_POSITION));
    elements.Push(VertexElement(TYPE_VECTOR3, SEM_NORMAL));
    vb->SetSize(verticesData.size()/6, elements);
    vb->SetData(verticesData.data());

    ib->SetShadowed(true);
    ib->SetSize(indexData.size(), false);
    ib->SetData(indexData.data());

    geom->SetVertexBuffer(0, vb);
    geom->SetIndexBuffer(ib);
    geom->SetDrawRange(TRIANGLE_LIST, 0, indexData.size());

    fromScratchModel->SetNumGeometries(1);
    fromScratchModel->SetGeometry(0, 0, geom);
    fromScratchModel->SetBoundingBox(BoundingBox(Vector3(-1.0f, -1.0f, -1.0f), Vector3(1.0f, 1.0f, 1.0f)));

    // Though not necessary to render, the vertex & index buffers must be listed in the model so that it can be saved properly
    Vector<SharedPtr<VertexBuffer> > vertexBuffers;
    Vector<SharedPtr<IndexBuffer> > indexBuffers;
    vertexBuffers.Push(vb);
    indexBuffers.Push(ib);
    // Morph ranges could also be not defined. Here we simply define a zero range (no morphing) for the vertex buffer
    PODVector<unsigned> morphRangeStarts;
    PODVector<unsigned> morphRangeCounts;
    morphRangeStarts.Push(0);
    morphRangeCounts.Push(0);
    fromScratchModel->SetVertexBuffers(vertexBuffers, morphRangeStarts, morphRangeCounts);
    fromScratchModel->SetIndexBuffers(indexBuffers);

    //Node* node = scene_->CreateChild("FromScratchObject");
    //node->SetPosition(Vector3(0.0f, 3.0f, 0.0f));
    StaticModel* object = pPlanetNode->CreateComponent<StaticModel>();
    object->SetModel(fromScratchModel);
	//Material *pMat =  object->GetMaterial(0);
	//pMat->SetFillMode(FILL_WIREFRAME);
	
	return pPlanetNode;
	
}


void BW::Planet::ConvertToDodecahedronMesh(std::vector<Vector3>& vertices, std::vector<IntVector3>& faces)
{
	std::vector<Vector3> verticesNew;

	
	Vector3 midpoint;
	
	std::vector<IntVector2> lines;
	
	std::vector<Vector3> verticesOut;
	std::vector<IntVector3> facesOut;
	
	
	

	
	for(const auto& vert: vertices)
	{
		midpoint += vert;
	}
	
	midpoint = midpoint/(static_cast<float>(vertices.size()));
	
	//{
	//	String str;
	//	str.AppendWithFormat("Midpoint: %f, %f, %f", midpoint.x_, midpoint.y_, midpoint.z_);
	//	Log::Write(LOG_INFO, str);
	//}
	
	// find neightbours
    for (int32_t i = 0; i < faces.size(); ++i)
    {
		const auto& face = faces[i];
		nbVert[face.x_].insert(face.y_);
		nbVert[face.x_].insert(face.z_);

		nbVert[face.y_].insert(face.x_);
		nbVert[face.y_].insert(face.z_);

		nbVert[face.z_].insert(face.x_);
		nbVert[face.z_].insert(face.y_);
		
		nbFaces[face.x_].insert(i);
		nbFaces[face.y_].insert(i);
		nbFaces[face.z_].insert(i);
		
	}
	
	//for (size_t i = 0; i < neightbours.size(); ++i)
	//{
	//	String str;
	//	str.AppendWithFormat("Neighbours for [%u]: ", i);
	//	for (const auto& n: neightbours[i])
	//	{
	//		str.AppendWithFormat(" %d,", n);
	//	}
	//	Log::Write(LOG_INFO, str);
	//}
	
	//create planes
	for(size_t i = 0; i < vertices.size(); ++i)
	{
		//create new polygon plane
		Vector3 mainNormal = vertices[i] - midpoint;
		Plane mainPlane(mainNormal, vertices[i]);
		
		vertPlanes[i] = mainPlane;
	}
	
	//create rays
	for(size_t i = 0; i < vertices.size(); ++i)
	{
		//create new polygon plane
		Plane &plane = vertPlanes[i];
		for()
	}	
	
	
	//change data
	//vertices.swap(verticesOut);
	//faces.swap(facesOut);
}
