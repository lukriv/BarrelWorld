#ifndef __BW_SCENE_MANAGER_H__
#define __BW_SCENE_MANAGER_H__

#include <stdint.h>

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Container/RefCounted.h>
#include <Urho3D/Graphics/Viewport.h>
#include "TerrainTile.h"
#include "ActualMission.h"


namespace Urho3D {
	class Camera;
}

namespace BW
{

	class WorldManager : public Urho3D::RefCounted
	{
		Urho3D::Application *m_pApp;
		
		Urho3D::WeakPtr<Urho3D::Scene> m_spMainScene;
		
		Urho3D::WeakPtr<Urho3D::Node> m_spAvatarNode;
		Urho3D::WeakPtr<Urho3D::Node> m_spBoxNode;
		Urho3D::WeakPtr<Urho3D::Node> m_spCameraNode;
		
		Urho3D::SharedPtr<TerrainTile> m_spTerrainMgr;
		Urho3D::SharedPtr<ActualMission> m_spActualMission;
		
		Urho3D::WeakPtr<Urho3D::Viewport> m_spViewport;
		
		
		bool m_characterMode;
		float m_cameraYaw;
		float m_cameraPitch;
		Urho3D::Vector3 m_cameraDistance;
		
	public:
		WorldManager(Urho3D::Application *pApp, Urho3D::WeakPtr<Urho3D::Scene> spMainScene);
		~WorldManager();
		
		void SetViewport(Urho3D::Viewport *pViewport);
		
		Urho3D::Scene* GetMainScene() { return m_spMainScene; }
		
		Urho3D::Camera* GetCamera();
		Urho3D::Node* GetCameraNode() { return m_spCameraNode; }
		
		
		
		void Update(float timeStep);
		
		
		void NewGame();
		
		void StoreGame();
		
	protected:
		void CreateCamera();
		void CreateLights();
		
		void GenerateEnemies();
		
		void SwitchCameraToCharacterMode();
		
		float GetAvatarYaw(int32_t x, int32_t y);

	};

}

#endif // __BW_SCENE_MANAGER_H__
