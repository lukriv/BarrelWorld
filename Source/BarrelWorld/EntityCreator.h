#ifndef __BARRELWORLD_ENTITY_CREATOR__H__
#define __BARRELWORLD_ENTITY_CREATOR__H__

namespace Urho3D {
	class Application;
	class Node;
	class String;
}

namespace BW
{

	class EntityCreator
	{
	public:
		static Urho3D::Node* CreateAvatar(const Urho3D::String &name, Urho3D::Application *pApp, Urho3D::Node *pScene);
		static Urho3D::Node* CreateAvatar2(const Urho3D::String &name, Urho3D::Application *pApp, Urho3D::Node *pScene);
		static Urho3D::Node* CreatePistol(const Urho3D::String &name, Urho3D::Application *pApp, Urho3D::Node *pScene);

	private:

	};

}

#endif // __BARRELWORLD_ENTITY_CREATOR__H__
