#ifndef __BARRELWORLD_DEBUG_TOOLS_H__
#define __BARRELWORLD_DEBUG_TOOLS_H__


namespace Urho3D {
	class Application;
	class Scene;
	class Node;
	class Model;
	class Material;
	class String;
	class VertexElement;
	class Variant;
}

namespace BW
{

	class DebugTools
	{
	public:
		static void LogModel(Urho3D::Model *pModel);
		static void LogModelGeometry(Urho3D::Model *pModel);
		static void LogModelSkeleton(Urho3D::Model *pModel);
		static void LogVertexElement(const Urho3D::VertexElement& pVertexElement, void* data);
		
		static void LogMaterial(Urho3D::Material *pMaterial);
		
		static void LogVariant(const Urho3D::Variant& variant);

	};

}

#endif // __BARRELWORLD_DEBUG_TOOLS_H__
