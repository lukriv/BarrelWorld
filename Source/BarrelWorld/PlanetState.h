#ifndef __BARREL_WORLD_GAME_STATE_BASE__H__
#define __BARREL_WORLD_GAME_STATE_BASE__H__

#include "StateBase.h"

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Viewport.h>

namespace BW
{

	class PlanetState : public StateBase
	{
	protected:
		Game *m_pApp;
		
		Urho3D::WeakPtr<Urho3D::Scene> m_spMainScene;
		
		Urho3D::WeakPtr<Urho3D::Node> m_spCameraNode;
		
		Urho3D::WeakPtr<Urho3D::Viewport> m_spViewport;
		
		
		float m_cameraYaw;
		float m_cameraPitch;
		Urho3D::Vector3 m_cameraDistance;
	public:
		
		virtual ~PlanetState();
				
		Urho3D::Camera* GetCamera();
		
		virtual void Init() override;
		
		virtual void Update(float timeStep) override;
		virtual void Cleanup() override;

		virtual void Pause() override;
		virtual void Resume() override;
		
	public:
		static PlanetState* GetInstance(Game *pApp);		
		
	protected:
		static PlanetState* gPlanetState;
	
	protected:
		PlanetState(Game *pApp);
		void CreateCamera();
	};

}

#endif // __BARREL_WORLD_GAME_STATE_BASE__H__
