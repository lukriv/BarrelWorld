#ifndef __BW_SCENE_MANAGER_H__
#define __BW_SCENE_MANAGER_H__

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Container/RefCounted.h>


namespace Urho3D {
	class Camera;
}

namespace BW
{

	class WorldManager : public Urho3D::RefCounted
	{
		Urho3D::Application *m_pApp;
		
		Urho3D::SharedPtr<Urho3D::Scene> m_spMainScene;
		
		Urho3D::SharedPtr<Urho3D::Node> m_spBoxNode;
		Urho3D::SharedPtr<Urho3D::Node> m_spCameraNode;
	public:
		WorldManager(Urho3D::Application *pApp, Urho3D::SharedPtr<Urho3D::Scene> spMainScene);
		~WorldManager();
		
		
		Urho3D::Scene* GetMainScene() { return m_spMainScene; }
		
		Urho3D::Camera* GetCamera();
		
		
		
		void Update(float timeStep);
		
		
		void NewGame();
		
		void StoreGame();
		
	protected:
		void CreateCamera();

	};

}

#endif // __BW_SCENE_MANAGER_H__
