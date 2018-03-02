#include "WorldManager.h"
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/IO/Log.h>
#include "Rotator.h"
#include "Character.h"
#include "EntityCreator.h"

using namespace Urho3D;


static const float DEFAULT_CAMERA_DISTANCE = 40;
static const float DEFAULT_CAMERA_YAW = 0;
static const float DEFAULT_CAMERA_PITCH = 70;


BW::WorldManager::WorldManager(Urho3D::Application *pApp, Urho3D::WeakPtr<Urho3D::Scene> spMainScene) : m_pApp(pApp)
	, m_spMainScene(spMainScene)
	, m_characterMode(true)
{
	ResourceCache* cache=pApp->GetSubsystem<ResourceCache>();
	
	
	m_cameraYaw = 0;
	m_cameraPitch = 0;
	
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

void BW::WorldManager::SetViewport(Urho3D::Viewport* pViewport)
{
	m_spViewport = pViewport;
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
	
	// switch camera mode - free and character
	if(input->GetKeyPress('O'))
	{
		m_characterMode = !m_characterMode;
		if(m_characterMode)
		{
			SwitchCameraToCharacterMode();
		}
	}
	
	if(m_characterMode && m_spAvatarNode.NotNull())
	{
		m_spCameraNode->SetPosition( m_spAvatarNode->GetPosition() + m_cameraDistance);
		
		BW::Character* m_controls = m_spAvatarNode->GetComponent<BW::Character>();

		m_controls->m_controls.Set(BW::CTRL_FORWARD | BW::CTRL_BACK | BW::CTRL_LEFT | BW::CTRL_RIGHT | BW::CTRL_JUMP, false);
		// character move
		if(input->GetKeyDown('W'))
			m_controls->m_controls.Set(BW::CTRL_FORWARD, true);
		if(input->GetKeyDown('S'))
			m_controls->m_controls.Set(BW::CTRL_BACK, true);
		if(input->GetKeyDown('A'))
			m_controls->m_controls.Set(BW::CTRL_LEFT, true);
		if(input->GetKeyDown('D'))
			m_controls->m_controls.Set(BW::CTRL_RIGHT, true);
			
			
		// get intersection between pointer and terrain
		//first get mouse pointer position
		IntVector2 mousePos = input->GetMousePosition();
		m_controls->m_controls.yaw_ = GetAvatarYaw( mousePos.x_, mousePos.y_);

		
	 
		
	} else {
	
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
				m_cameraYaw+=MOUSE_SENSITIVITY*mouseMove.x_;
				m_cameraPitch+=MOUSE_SENSITIVITY*mouseMove.y_;
				m_cameraPitch=Clamp(m_cameraPitch,-90.0f,90.0f);
				// Reset rotation and set yaw and pitch again
				m_spCameraNode->SetDirection(Vector3::FORWARD);
				m_spCameraNode->Yaw(m_cameraYaw);
				m_spCameraNode->Pitch(m_cameraPitch);
			}
		}
	
	}
}

void BW::WorldManager::NewGame()
{

	m_spMainScene->RemoveAllChildren();
	CreateCamera();
	// set new camera to viewport
	m_spViewport->SetCamera(GetCamera());
	
	CreateLights();
	
	if(m_spTerrainMgr.Null())
	{
		m_spTerrainMgr = new TerrainTile(m_pApp, m_spMainScene);
	}
	m_spTerrainMgr->GenerateTerrain();
	
	Log::Write(LOG_INFO, String("Terrain was generated"));
	
	Vector3 vec = m_spTerrainMgr->GetTerrainNode()->GetPosition();
	Vector3 trans = vec + Vector3(0,50,0);
	
	m_spAvatarNode = EntityCreator::CreateAvatar("Avatar", m_pApp, m_spMainScene);
	
	m_spAvatarNode->SetPosition(vec + Vector3(0,25,0));
	
	String str;
	str.AppendWithFormat("Terrain position: %f, %f, %f", vec.x_, vec.y_, vec.z_);
	Log::Write(LOG_INFO, str);
	
	m_spCameraNode->Translate(trans);
	
	if(m_characterMode)
	{
		SwitchCameraToCharacterMode();
	}
	
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
	//camera->SetOrthographic(true);
	m_spCameraNode->SetDirection(Vector3::FORWARD);
	m_spCameraNode->Yaw(m_cameraYaw);
	m_spCameraNode->Pitch(m_cameraPitch);
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

void BW::WorldManager::SwitchCameraToCharacterMode()
{
	// z = y / tan pitch
	m_cameraDistance.x_ = 0;
	m_cameraDistance.y_ = DEFAULT_CAMERA_DISTANCE;
	m_cameraDistance.z_ = - (DEFAULT_CAMERA_DISTANCE / Tan(DEFAULT_CAMERA_PITCH));
	m_spCameraNode->SetPosition( m_spAvatarNode->GetPosition() + m_cameraDistance);
	m_cameraPitch = DEFAULT_CAMERA_PITCH;
	m_cameraYaw = DEFAULT_CAMERA_YAW;
	m_spCameraNode->SetDirection(Vector3::FORWARD);
	m_spCameraNode->Yaw(m_cameraYaw);
	m_spCameraNode->Pitch(m_cameraPitch);
}

float BW::WorldManager::GetAvatarYaw( int32_t x, int32_t y)
{
	//static int32_t calls = 0;
	
	Vector3 origin = m_spAvatarNode->GetPosition();
	
	Plane plane(Vector3(0,1,0), origin);
	
	Ray ray = m_spViewport->GetScreenRay(x, y);
	
	Vector3 resultVec = ((ray.direction_ * ray.HitDistance(plane)) + ray.origin_) - origin;
	
	float yaw = resultVec.Angle(Vector3(0,0,1));
	
	yaw = (resultVec.x_ > 0) ? yaw : -yaw; 
	
//	if(calls > 100)
//	{
//		String str;
//		str.AppendWithFormat("Yaw: %f; x=%f", yaw, resultVec.x_);
//		Log::Write(LOG_INFO, str);
//		calls = 0;
//	} else {
//		++calls;
//	}
	
	
	return yaw;
	
//	PhysicsRaycastResult result;
//	
//	PhysicsWorld *pPhysicsWorld = m_spMainScene->GetComponent<PhysicsWorld>();
//	
//	pPhysicsWorld->RaycastSingleSegmented (result, ray, 20000, 100);
	
//	if(calls > 100)
//	{
//		String str;
//		str.AppendWithFormat("Ray intersection: distance (%f), bodyAddr(0x%x), intersection( %f, %f, %f)"
//			, result.distance_, (int64_t) result.body_, result.position_.x_, result.position_.y_, result.position_.z_);
//		Log::Write(LOG_INFO, str);
//		calls = 0;
//	} else {
//		++calls;
//	}
	
	
}

