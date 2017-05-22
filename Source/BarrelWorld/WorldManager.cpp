#include "WorldManager.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>

using namespace Urho3D;

BW::WorldManager::WorldManager(Urho3D::Application *pApp, Urho3D::SharedPtr<Urho3D::Scene> spMainScene) : m_pApp(pApp), m_spMainScene(spMainScene)
{
	ResourceCache* cache=pApp->GetSubsystem<ResourceCache>();
	
	
	// Let's put a box in there.
    m_spBoxNode=m_spMainScene->CreateChild("Box");
    m_spBoxNode->SetPosition(Vector3(0,0,5));
    StaticModel* boxObject=m_spBoxNode->CreateComponent<StaticModel>();
    boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
    boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
	
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
	
	        // We need a camera from which the viewport can render.
	m_spCameraNode=m_spMainScene->CreateChild("Camera");
    Camera* camera=m_spCameraNode->CreateComponent<Camera>();
    camera->SetFarClip(2000);
}

BW::WorldManager::~WorldManager()
{
}




Urho3D::Camera* BW::WorldManager::GetCamera()
{
	return m_spCameraNode->GetComponent<Camera>();
}

void BW::WorldManager::Update(float timeStep)
{
	// Rotate the box thingy.
    // A much nicer way of doing this would be with a LogicComponent.
    // With LogicComponents it is easy to control things like movement
    // and animation from some IDE, console or just in game.
    // Alas, it is out of the scope for our simple example.
	m_spBoxNode->Rotate(Quaternion(8*timeStep,16*timeStep,0));
}
