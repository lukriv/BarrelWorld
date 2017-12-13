#ifndef __BARREL_WORLD_CHARACTER_LOGIC__H__
#define __BARREL_WORLD_CHARACTER_LOGIC__H__

#include <Urho3D/Scene/LogicComponent.h>

namespace BW
{
	
	enum ControlEnum {
		CTRL_FORWARD = 1,
		CTRL_BACK = 2,
		CTRL_LEFT = 4,
		CTRL_RIGHT = 8,
		CTRL_JUMP = 16
	}

	const float MOVE_FORCE = 0.8f;
	const float INAIR_MOVE_FORCE = 0.02f;
	const float BRAKE_FORCE = 0.2f;
	const float JUMP_FORCE = 7.0f;
	const float YAW_SENSITIVITY = 0.1f;
	const float INAIR_THRESHOLD_TIME = 0.1f;
	
	/// Character component, responsible for physical movement according to controls
	class Character : public Urho3D::LogicComponent
	{
		URHO3D_OBJECT(Character, Urho3D::LogicComponent);

	public:
		/// Register object factory and attributes.
		static void RegisterObject(Urho3D::Context* context);

	public:
		/// Construct.
		Character(Urho3D::Context* context);
	
		/// Handle startup. Called by LogicComponent base class.
		virtual void Start();
		/// Handle physics world update. Called by LogicComponent base class.
		virtual void FixedUpdate(float timeStep);
		
		/// Movement controls. Assigned by the main program each frame.
		Urho3D::Controls controls_;
		
	private:
		/// Handle physics collision event.
		void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
		
		/// Grounded flag for movement.
		bool onGround_;
		/// Jump flag.
		bool okToJump_;
		/// In air timer. Due to possible physics inaccuracy, character can be off ground for max. 1/10 second and still be allowed to move.
		float inAirTimer_;
	};

}

#endif //__BARREL_WORLD_CHARACTER_LOGIC__H__