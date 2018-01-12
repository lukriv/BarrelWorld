#ifndef __BARRELWORLD_ENTITY_CREATOR__H__
#define __BARRELWORLD_ENTITY_CREATOR__H__

namespace Urho3D {
	class Application;
	class Scene;
	class Node;
	class Model;
	class String;
	class VertexElement;
}

namespace BW
{

	class EntityCreator
	{
	public:
		static Urho3D::Node* CreateAvatar(const Urho3D::String &name, Urho3D::Application *pApp, Urho3D::Scene *pScene);
		static Urho3D::Node* CreateAvatar2(const Urho3D::String &name, Urho3D::Application *pApp, Urho3D::Scene *pScene);

	private:
		static void LogModel(Urho3D::Model *pModel);
		static void LogModelGeometry(Urho3D::Model *pModel);
		static void LogModelSkeleton(Urho3D::Model *pModel);
		static void LogVertexElement(const Urho3D::VertexElement& pVertexElement, void* data);
	};

}

#endif // __BARRELWORLD_ENTITY_CREATOR__H__
