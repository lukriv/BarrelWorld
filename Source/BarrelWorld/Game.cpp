#include "Game.h"

#include <string>
#include <sstream>
 
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Physics/PhysicsWorld.h>
//#include "WorldManager.h"
#include "PlanetState.h"

#include "Rotator.h"
#include "Character.h"

using namespace Urho3D;

namespace BW {


    /**
    * This happens before the engine has been initialized
    * so it's usually minimal code setting defaults for
    * whatever instance variables you have.
    * You can also do this in the Setup method.
    */
    Game::Game(Context * context) : Application(context)
		,framecount_(0)
		,time_(0)
	#ifdef _DEBUG_
		, debugMode_(false)
	#endif //_DEBUG_
		, m_actualState(nullptr)
    {
		BW::Rotator::RegisterObject(context);
		BW::Character::RegisterObject(context);
    }
 
    /**
    * This method is called _before_ the engine has been initialized.
    * Thusly, we can setup the engine parameters before anything else
    * of engine importance happens (such as windows, search paths,
    * resolution and other things that might be user configurable).
    */
    void Game::Setup()
    {
        // These parameters should be self-explanatory.
        // See http://urho3d.github.io/documentation/1.32/_main_loop.html
        // for a more complete list.
        engineParameters_["FullScreen"]=false;
        engineParameters_["WindowWidth"]=1280;
        engineParameters_["WindowHeight"]=960;
        engineParameters_["WindowResizable"]=true;
        // Override the resource prefix path to use. "If not specified then the
        // default prefix path is set to URHO3D_PREFIX_PATH environment
        // variable (if defined) or executable path."
        // By default mine was in /usr/local/share, change as needed.
        // Remember to use a TRAILING SLASH to your path! (for unknown reason)
        //engineParameters_["ResourcePrefixPath"] = "/usr/local/share/Urho3D/Bin/";
    }
 
    /**
    * This method is called _after_ the engine has been initialized.
    * This is where you set up your actual content, such as scenes,
    * models, controls and what not. Basically, anything that needs
    * the engine initialized and ready goes in here.
    */
    void Game::Start()
    {
        // We will be needing to load resources.
        // All the resources used in this example comes with Urho3D.
        // If the engine can't find them, check the ResourcePrefixPath.
        ResourceCache* cache=GetSubsystem<ResourceCache>();
		
		cache->AddResourceDir(String("BarrelData"));
 
        // Seems like the mouse must be in cursor mode before creating the UI or it won't work.
        GetSubsystem<Input>()->SetMouseVisible(true);
        GetSubsystem<Input>()->SetMouseGrabbed(false);
 
        // Let's use the default style that comes with Urho3D.
        GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

        // Add main menu
		m_spMainmenu = new BW::MainMenu(this, engineParameters_["WindowWidth"].GetInt(), engineParameters_["WindowHeight"].GetInt());

 
        // Let's setup a scene to render.
        scene_=new Scene(context_);
        // Let the scene have an Octree component!
        scene_->CreateComponent<Octree>();
		scene_->CreateComponent<PhysicsWorld>();
        // Let's add an additional scene component for fun.
	#ifdef _DEBUG_
        scene_->CreateComponent<DebugRenderer>();
	#endif //_DEBUG_

		//m_actualState = BW::WorldManager::GetInstance(this);
		ChangeState(BW::PlanetState::GetInstance(this));
 
		// Now we setup the viewport. Ofcourse, you can have more than one!

 
        // We subscribe to the events we'd like to handle.
        // In this example we will be showing what most of them do,
        // but in reality you would only subscribe to the events
        // you really need to handle.
        // These are sort of subscribed in the order in which the engine
        // would send the events. Read each handler method's comment for
        // details.
        SubscribeToEvent(E_BEGINFRAME,URHO3D_HANDLER(Game,HandleBeginFrame));
        SubscribeToEvent(E_KEYDOWN,URHO3D_HANDLER(Game,HandleKeyDown));
        SubscribeToEvent(E_UIMOUSECLICK,URHO3D_HANDLER(Game,HandleControlClicked));
        SubscribeToEvent(E_UPDATE,URHO3D_HANDLER(Game,HandleUpdate));
        SubscribeToEvent(E_POSTUPDATE,URHO3D_HANDLER(Game,HandlePostUpdate));
        SubscribeToEvent(E_RENDERUPDATE,URHO3D_HANDLER(Game,HandleRenderUpdate));
        SubscribeToEvent(E_POSTRENDERUPDATE,URHO3D_HANDLER(Game,HandlePostRenderUpdate));
        SubscribeToEvent(E_ENDFRAME,URHO3D_HANDLER(Game,HandleEndFrame));
    }
 
    /**
    * Good place to get rid of any system resources that requires the
    * engine still initialized. You could do the rest in the destructor,
    * but there's no need, this method will get called when the engine stops,
    * for whatever reason (short of a segfault).
    */
    void Game::Stop()
    {
		m_spMainmenu.Reset();
    }
 
    /**
    * Every frame's life must begin somewhere. Here it is.
    */
    void Game::HandleBeginFrame(StringHash eventType,VariantMap& eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }
 
    /**
    * Input from keyboard is handled here. I'm assuming that Input, if
    * available, will be handled before E_UPDATE.
    */
    void Game::HandleKeyDown(StringHash eventType,VariantMap& eventData)
    {
        using namespace KeyDown;
        int key=eventData[P_KEY].GetInt();
        if(key==KEY_ESCAPE)
		{
			m_spMainmenu->Visible(!m_spMainmenu->IsVisible());
		}
 
        if(key==KEY_TAB)
        {
            GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
            GetSubsystem<Input>()->SetMouseGrabbed(!GetSubsystem<Input>()->IsMouseGrabbed());
        }
    }
 
    /**
    * You can get these events from when ever the user interacts with the UI.
    */
    void Game::HandleControlClicked(StringHash eventType,VariantMap& eventData)
    {
        // Query the clicked UI element.
        UIElement* clicked=static_cast<UIElement*>(eventData[UIMouseClick::P_ELEMENT].GetPtr());
        if(clicked)
		{
            if(clicked->GetName()=="exit")   // check if the quit button was clicked
			{
                engine_->Exit();
			} else if(clicked->GetName()=="new") {
				ChangeState(PlanetState::GetInstance(this));
				m_spMainmenu->Visible(false);
			} else if(clicked->GetName()=="store") {
				//m_actualState->StoreGame();
			}
		}
    }
    /**
    * Your non-rendering logic should be handled here.
    * This could be moving objects, checking collisions and reaction, etc.
    */
    void Game::HandleUpdate(StringHash eventType,VariantMap& eventData)
    {
        float timeStep=eventData[Update::P_TIMESTEP].GetFloat();
        framecount_++;
        time_+=timeStep;
 
        if(time_ >=1)
        {
            time_=0;
        }
		
	#ifdef _DEBUG_
		if(GetSubsystem<Input>()->GetKeyPress('P'))
		{
			debugMode_ = !debugMode_;
		}
	#endif //_DEBUG_	
		m_actualState->Update(timeStep);

    }
    /**
    * Anything in the non-rendering logic that requires a second pass,
    * it might be well suited to be handled here.
    */
    void Game::HandlePostUpdate(StringHash eventType,VariantMap& eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }
    /**
    * If you have any details you want to change before the viewport is
    * rendered, try putting it here.
    * See http://urho3d.github.io/documentation/1.32/_rendering.html
    * for details on how the rendering pipeline is setup.
    */
    void Game::HandleRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }
    /**
    * After everything is rendered, there might still be things you wish
    * to add to the rendering. At this point you cannot modify the scene,
    * only post rendering is allowed. Good for adding things like debug
    * artifacts on screen or brush up lighting, etc.
    */
    void Game::HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
        // We could draw some debuggy looking thing for the octree.
	#ifdef _DEBUG_
		if(debugMode_)
		{
			//scene_->GetComponent<Octree>()->DrawDebugGeometry(true);
			scene_->GetComponent<PhysicsWorld>()->DrawDebugGeometry(true);
		}
	#endif //_DEBUG_
    }
    /**
    * All good things must come to an end.
    */
    void Game::HandleEndFrame(StringHash eventType,VariantMap& eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }
	
	void Game::ChangeState(StateBase *state)
	{
		if(m_actualState) m_actualState->Cleanup();
		m_actualState = state;
		m_actualState->Init();
	}
	
	Urho3D::WeakPtr<Urho3D::Scene> Game::GetMainScene()
	{
		return scene_;
	}
};



