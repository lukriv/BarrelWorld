#ifndef __BARREL_WORLD_CHARACTER_LOGIC__H__
#define __BARREL_WORLD_CHARACTER_LOGIC__H__

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Input/Controls.h>

namespace BW
{
	
	enum ControlEnum {
		CTRL_FORWARD = 1,
		CTRL_BACK = 2,
		CTRL_LEFT = 4,
		CTRL_RIGHT = 8,
		CTRL_JUMP = 16
	};

	extern const float MOVE_FORCE;
	extern const float INAIR_MOVE_FORCE;
	extern const float BRAKE_FORCE;
	extern const float JUMP_FORCE;
	extern const float YAW_SENSITIVITY;
	extern const float INAIR_THRESHOLD_TIME;
	
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
		Urho3D::Controls m_controls;
		
	private:
		/// Handle physics collision event.
		void HandleNodeCollision(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
		
		/// Grounded flag for movement.
		bool m_onGround;
		/// Jump flag.
		bool m_okToJump;
		/// In air timer. Due to possible physics inaccuracy, character can be off ground for max. 1/10 second and still be allowed to move.
		float m_inAirTimer;
	};

}

#endif //__BARREL_WORLD_CHARACTER_LOGIC__H__