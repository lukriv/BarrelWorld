#ifndef __BARREL_WORLD_MAIN_MENU_STATE__H__
#define __BARREL_WORLD_MAIN_MENU_STATE__H__

#include "state.h"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Engine.h>

#include "mainmenu.h"

namespace BW
{

	class StateMainMenu : public State
	{
	private:
		Urho3D::Application *m_pApp;
		Urho3D::SharedPtr<BW::MainMenu> m_spMainmenu;
	public:
		StateMainMenu(Urho3D::Application *pApp, StateHandler *pHandler);
		~StateMainMenu();
		
		
		
		virtual bool Start() override;
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
		void HandleEndFrame(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	
	};

}

#endif // __BARREL_WORLD_MAIN_MENU_STATE__H__
