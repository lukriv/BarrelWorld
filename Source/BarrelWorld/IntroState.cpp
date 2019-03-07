#include "IntroState.h"
#include "Game.h"

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Camera.h>
#include "Rotator.h"

using namespace Urho3D;

BW::IntroState* BW::IntroState::gIntroState = nullptr;

BW::IntroState* BW::IntroState::GetInstance(Game* pApp)
{
	if(gIntroState == nullptr)
	{
		gIntroState = new BW::IntroState(pApp);
	}
	
	return gIntroState;
}

///
BW::IntroState::IntroState(Game* pApp) : m_pApp(pApp), m_spMainScene(pApp->GetMainScene())
{
	
}

BW::IntroState::~IntroState()
{
}


void BW::IntroState::Init()
{
	CreateCamera();
	// set new camera to viewport
	ResourceCache* cache=m_pApp->GetSubsystem<ResourceCache>();
	
	Renderer* pRenderer= m_pApp->GetSubsystem<Renderer>();
	SharedPtr<Viewport> viewport(new Viewport( m_pApp->GetContext(), m_spMainScene, GetCamera()));
	pRenderer->SetViewport(0,viewport);
	m_spViewport = viewport;
	
	m_spViewport->SetCamera(GetCamera());
	
    Urho3D::Node* pBoxNode=m_spMainScene->CreateChild("Box");
    pBoxNode->SetPosition(Vector3(0,0,5));
    StaticModel* boxObject=pBoxNode->CreateComponent<StaticModel>();
    boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
	pBoxNode->CreateComponent<Rotator>();
	
	// Create two lights
    {
        Node* lightNode=m_spMainScene->CreateChild("Light");
        lightNode->SetPosition(Vector3(-5,0,10));
        Light* light=lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_POINT);
        light->SetRange(50);
        light->SetBrightness(1.2);
        light->SetColor(Color(1,.5,.8,1));
        light->SetCastShadows(true);
    }
    {
        Node* lightNode=m_spMainScene->CreateChild("Light");
        lightNode->SetPosition(Vector3(5,0,10));
        Light* light=lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_POINT);
        light->SetRange(50);
        light->SetBrightness(1.2);
        light->SetColor(Color(.5,.8,1,1));
        light->SetCastShadows(true);
    }
}

void BW::IntroState::Cleanup()
{
	m_spMainScene->RemoveAllChildren();
}

void BW::IntroState::Pause()
{
}

void BW::IntroState::Resume()
{
}

void BW::IntroState::Update(float timeStep)
{
}





void BW::IntroState::CreateCamera()
{
	m_spCameraNode=m_spMainScene->CreateChild("Camera");
    Camera* camera=m_spCameraNode->CreateComponent<Camera>();
    camera->SetFarClip(20000);
	//camera->SetOrthographic(true);
	m_spCameraNode->SetDirection(Vector3::FORWARD);
	m_spCameraNode->Yaw(0);
	m_spCameraNode->Pitch(0);
}

Urho3D::Camera* BW::IntroState::GetCamera()
{
	return m_spCameraNode->GetComponent<Camera>();
}
