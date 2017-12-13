#include "WorldManager.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/Log.h>
#include "Rotator.h"
#include "EntityCreator.h"

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
	m_spBoxNode->CreateComponent<Rotator>();
	
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
	CreateCamera();

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
	static const float MOUSE_SENSITIVITY=0.1f;
	float MOVE_SPEED=10.0f;
	Input* input=m_pApp->GetSubsystem<Input>();
    if(input->GetQualifierDown(1))  // 1 is shift, 2 is ctrl, 4 is alt
        MOVE_SPEED*=10;
    if(input->GetKeyDown('W'))
        m_spCameraNode->Translate(Vector3(0,0, 1)*MOVE_SPEED*timeStep);
    if(input->GetKeyDown('S'))
        m_spCameraNode->Translate(Vector3(0,0,-1)*MOVE_SPEED*timeStep);
    if(input->GetKeyDown('A'))
        m_spCameraNode->Translate(Vector3(-1,0,0)*MOVE_SPEED*timeStep);
    if(input->GetKeyDown('D'))
        m_spCameraNode->Translate(Vector3( 1,0,0)*MOVE_SPEED*timeStep);
 
    if(input->GetMouseButtonDown(MOUSEB_RIGHT))
    {
        // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
        IntVector2 mouseMove=input->GetMouseMove();
        // avoid the weird extrem values before moving the mouse
        if(mouseMove.x_>-2000000000&&mouseMove.y_>-2000000000)
        {
            static float yaw_=0;
            static float pitch_=0;
            yaw_+=MOUSE_SENSITIVITY*mouseMove.x_;
            pitch_+=MOUSE_SENSITIVITY*mouseMove.y_;
            pitch_=Clamp(pitch_,-90.0f,90.0f);
            // Reset rotation and set yaw and pitch again
            m_spCameraNode->SetDirection(Vector3::FORWARD);
            m_spCameraNode->Yaw(yaw_);
            m_spCameraNode->Pitch(pitch_);
        }
    }
}

void BW::WorldManager::NewGame()
{

	m_spMainScene->RemoveAllChildren();
	CreateCamera();
	
	CreateLights();
	
	m_spTerrainMgr = new TerrainManager(m_pApp, m_spMainScene);
	m_spTerrainMgr->GenerateTerrain();
	
	Log::Write(LOG_INFO, String("Terrain was generated"));
	
	Vector3 vec = m_spTerrainMgr->GetTerrainNode()->GetPosition();
	Vector3 trans = vec + Vector3(0,25,0);
	
	m_spAvatarNode = EntityCreator::CreateAvatar("Avatar", m_pApp, m_spMainScene);
	
	m_spAvatarNode->SetPosition(vec + Vector3(0,2,0));
	
	String str;
	str.AppendWithFormat("Terrain position: %f, %f, %f", vec.x_, vec.y_, vec.z_);
	Log::Write(LOG_INFO, str);
	
	m_spCameraNode->Translate(trans);
	
	m_spCameraNode->LookAt(vec);
	
	
	
}

void BW::WorldManager::StoreGame()
{
	Urho3D::File file(m_pApp->GetContext(), String("save01.xml"), FILE_WRITE);
	m_spMainScene->SaveXML(file);
}

void BW::WorldManager::CreateCamera()
{
	m_spCameraNode=m_spMainScene->CreateChild("Camera");
    Camera* camera=m_spCameraNode->CreateComponent<Camera>();
    camera->SetFarClip(20000);
}


void BW::WorldManager::CreateLights()
{
	Node* lightNode=m_spMainScene->CreateChild("Light");
	lightNode->SetDirection(Vector3(50,-50,50));
    lightNode->SetPosition(Vector3(50,50,50));
    Light* light=lightNode->CreateComponent<Light>();
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetRange(500);
    light->SetBrightness(1.2);
    light->SetColor(Color(1,1,1,1));
    light->SetCastShadows(true);
}
