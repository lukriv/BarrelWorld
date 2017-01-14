#include "statemainmenu.h"
#include "bwdefs.h"

using namespace Urho3D;

BW::StateMainMenu::StateMainMenu(Urho3D::Application *pApp, BW::StateHandler *pHandler) : BW::State(pHandler, BW_STATE_MAIN_MENU), m_pApp(pApp)
{
}

BW::StateMainMenu::~StateMainMenu()
{
}

bool BW::StateMainMenu::Start()
{
	m_pApp->GetEventDataMap()
	m_spMainmenu = new BW::MainMenu(this, engineParameters_["WindowWidth"].GetInt(), engineParameters_["WindowHeight"].GetInt());


	// Let's setup a scene to render.
	scene_=new Scene(context_);
	// Let the scene have an Octree component!
	scene_->CreateComponent<Octree>();
	// Let's add an additional scene component for fun.
	//scene_->CreateComponent<DebugRenderer>();

	// Let's put a box in there.
	boxNode_=scene_->CreateChild("Box");
	boxNode_->SetPosition(Vector3(0,0,5));
	StaticModel* boxObject=boxNode_->CreateComponent<StaticModel>();
	boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));


	// We need a camera from which the viewport can render.
	cameraNode_=scene_->CreateChild("Camera");
	Camera* camera=cameraNode_->CreateComponent<Camera>();
	camera->SetFarClip(2000);

	// Create two lights
	{
		Node* lightNode=scene_->CreateChild("Light");
		lightNode->SetPosition(Vector3(-5,0,10));
		Light* light=lightNode->CreateComponent<Light>();
		light->SetLightType(LIGHT_POINT);
		light->SetRange(50);
		light->SetBrightness(1.2);
		light->SetColor(Color(1,.5,.8,1));
		light->SetCastShadows(true);
	}
	{
		Node* lightNode=scene_->CreateChild("Light");
		lightNode->SetPosition(Vector3(5,0,10));
		Light* light=lightNode->CreateComponent<Light>();
		light->SetLightType(LIGHT_POINT);
		light->SetRange(50);
		light->SetBrightness(1.2);
		light->SetColor(Color(.5,.8,1,1));
		light->SetCastShadows(true);
	}

	// Now we setup the viewport. Ofcourse, you can have more than one!
	Renderer* renderer=GetSubsystem<Renderer>();
	SharedPtr<Viewport> viewport(new Viewport(context_,scene_,cameraNode_->GetComponent<Camera>()));
	renderer->SetViewport(0,viewport);
	
	SubscribeToEvent(E_BEGINFRAME,URHO3D_HANDLER( BW::StateMainMenu, HandleBeginFrame ));
	SubscribeToEvent(E_KEYDOWN,URHO3D_HANDLER( BW::StateMainMenu, HandleKeyDown ));
	SubscribeToEvent(E_UIMOUSECLICK,URHO3D_HANDLER( BW::StateMainMenu, HandleControlClicked ));
	SubscribeToEvent(E_UPDATE,URHO3D_HANDLER( BW::StateMainMenu, HandleUpdate ));
	SubscribeToEvent(E_POSTUPDATE,URHO3D_HANDLER( BW::StateMainMenu, HandlePostUpdate ));
	SubscribeToEvent(E_RENDERUPDATE,URHO3D_HANDLER( BW::StateMainMenu, HandleRenderUpdate ));
	SubscribeToEvent(E_POSTRENDERUPDATE,URHO3D_HANDLER( BW::StateMainMenu, HandlePostRenderUpdate ));
	SubscribeToEvent(E_ENDFRAME,URHO3D_HANDLER( BW::StateMainMenu, HandleEndFrame ));
	return false;
}

void BW::StateMainMenu::Stop()
{
}

void BW::StateMainMenu::HandleBeginFrame(StringHash eventType, VariantMap& eventData)
{
}

void BW::StateMainMenu::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
}

void BW::StateMainMenu::HandleControlClicked(StringHash eventType, VariantMap& eventData)
{
}

void BW::StateMainMenu::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
}

void BW::StateMainMenu::HandlePostUpdate(StringHash eventType, VariantMap& eventData)
{
}

void BW::StateMainMenu::HandleRenderUpdate(StringHash eventType, VariantMap& eventData)
{
}

void BW::StateMainMenu::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
}

void BW::StateMainMenu::HandleEndFrame(StringHash eventType, VariantMap& eventData)
{
}
BW::StateMainMenu::StateMainMenu(Urho3D::Application* pApp, StateHandler* pHandler)
{
}
