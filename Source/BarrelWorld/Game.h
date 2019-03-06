#ifndef __BARREL_WORLD_GAME_CLASS__H__
#define __BARREL_WORLD_GAME_CLASS__H__

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>

#include "mainmenu.h"
#include "StateBase.h"


namespace BW
{

/**
* Using the convenient Application API we don't have
* to worry about initializing the engine or writing a main.
* You can probably mess around with initializing the engine
* and running a main manually, but this is convenient and portable.
*/
class Game : public Urho3D::Application
{
public:
	int framecount_;
    float time_;
#ifdef _DEBUG_	
	bool debugMode_;
#endif //_DEBUG_
    Urho3D::SharedPtr<Urho3D::Scene> scene_;
    
	Urho3D::SharedPtr<BW::MainMenu> m_spMainmenu;
	//Urho3D::SharedPtr<BW::WorldManager> m_spWorldMgr;
	
	std::vector<StateBase*> m_stateStack;
	StateBase *m_actualState;

public: 
    /**
    * This happens before the engine has been initialized
    * so it's usually minimal code setting defaults for
    * whatever instance variables you have.
    * You can also do this in the Setup method.
    */
    Game(Urho3D::Context * context);
 
    /**
    * This method is called _before_ the engine has been initialized.
    * Thusly, we can setup the engine parameters before anything else
    * of engine importance happens (such as windows, search paths,
    * resolution and other things that might be user configurable).
    */
    virtual void Setup() override;
 
    /**
    * This method is called _after_ the engine has been initialized.
    * This is where you set up your actual content, such as scenes,
    * models, controls and what not. Basically, anything that needs
    * the engine initialized and ready goes in here.
    */
    virtual void Start() override;
 
    /**
    * Good place to get rid of any system resources that requires the
    * engine still initialized. You could do the rest in the destructor,
    * but there's no need, this method will get called when the engine stops,
    * for whatever reason (short of a segfault).
    */
    virtual void Stop() override;
 
    /**
    * Every frame's life must begin somewhere. Here it is.
    */
    void HandleBeginFrame(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);
 
    /**
    * Input from keyboard is handled here. I'm assuming that Input, if
    * available, will be handled before E_UPDATE.
    */
    void HandleKeyDown(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);
 
    /**
    * You can get these events from when ever the user interacts with the UI.
    */
    void HandleControlClicked(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);

    /**
    * Your non-rendering logic should be handled here.
    * This could be moving objects, checking collisions and reaction, etc.
    */
    void HandleUpdate(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);

    /**
    * Anything in the non-rendering logic that requires a second pass,
    * it might be well suited to be handled here.
    */
    void HandlePostUpdate(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);

    /**
    * If you have any details you want to change before the viewport is
    * rendered, try putting it here.
    * See http://urho3d.github.io/documentation/1.32/_rendering.html
    * for details on how the rendering pipeline is setup.
    */
    void HandleRenderUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap & eventData);

    /**
    * After everything is rendered, there might still be things you wish
    * to add to the rendering. At this point you cannot modify the scene,
    * only post rendering is allowed. Good for adding things like debug
    * artifacts on screen or brush up lighting, etc.
    */
    void HandlePostRenderUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap & eventData);

    /**
    * All good things must come to an end.
    */
    void HandleEndFrame(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);
	
	void ChangeState(StateBase *state);
	Urho3D::WeakPtr<Urho3D::Scene> GetMainScene();
};

}

#endif // __BARREL_WORLD_GAME_CLASS__H__
