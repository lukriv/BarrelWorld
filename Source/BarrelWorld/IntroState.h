#ifndef __BARREL_WORLD_INTRO_STATE__H__
#define __BARREL_WORLD_INTRO_STATE__H__

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Viewport.h>
#include "StateBase.h"

namespace BW
{

	class IntroState : public StateBase
	{
	protected:
		Game *m_pApp;
		
		Urho3D::WeakPtr<Urho3D::Scene> m_spMainScene;
		
		Urho3D::WeakPtr<Urho3D::Node> m_spCameraNode;
		
		Urho3D::WeakPtr<Urho3D::Viewport> m_spViewport;
	public:
		
		~IntroState();
		
		virtual void Init();
		virtual void Cleanup();

		virtual void Pause();
		virtual void Resume();

		virtual void Update(float timeStep);
		
	protected:
		static IntroState* gIntroState;
		
		IntroState(Game *pApp);
		
	public:
		static IntroState* GetInstance(Game *pApp);
		void CreateCamera();
		Urho3D::Camera* GetCamera();
	};

}

#endif // __BARREL_WORLD_INTRO_STATE__H__
