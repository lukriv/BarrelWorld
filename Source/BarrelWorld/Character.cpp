#include "Character.h"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/IO/MemoryBuffer.h>
#include <Urho3D/IO/Log.h>

namespace BW {
	const float MOVE_FORCE = 50.0f;
	const float INAIR_MOVE_FORCE = 0.02f;
	const float BRAKE_FORCE = 10.0f;
	const float JUMP_FORCE = 7.0f;
	const float YAW_SENSITIVITY = 0.1f;
	const float INAIR_THRESHOLD_TIME = 0.1f;
}

using namespace Urho3D;

void BW::Character::RegisterObject(Urho3D::Context* context)
{
	context->RegisterFactory<Character>();

    // These macros register the class attributes to the Context for automatic load / save handling.
    // We specify the Default attribute mode which means it will be used both for saving into file, and network replication
    /*URHO3D_ATTRIBUTE("Controls Yaw", float, controls_.yaw_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("Controls Pitch", float, controls_.pitch_, 0.0f, AM_DEFAULT);
    URHO3D_ATTRIBUTE("On Ground", bool, onGround_, false, AM_DEFAULT);
    URHO3D_ATTRIBUTE("OK To Jump", bool, okToJump_, true, AM_DEFAULT);
    URHO3D_ATTRIBUTE("In Air Timer", float, inAirTimer_, 0.0f, AM_DEFAULT);*/
}

BW::Character::Character(Urho3D::Context* context) : LogicComponent(context)
    , m_onGround(true)
    , m_okToJump(false)
    , m_inAirTimer(0.0f)
{
    // Only the physics update event is needed: unsubscribe from the rest for optimization
    SetUpdateEventMask(USE_FIXEDUPDATE);
}

void BW::Character::Start()
{
	// Component has been inserted into its scene node. Subscribe to events now
    SubscribeToEvent( GetNode(), E_NODECOLLISION, URHO3D_HANDLER(BW::Character, HandleNodeCollision));
}

void BW::Character::FixedUpdate(float timeStep)
{
	/// \todo Could cache the components for faster access instead of finding them each frame
    RigidBody* body = GetComponent<RigidBody>();
    //AnimationController* animCtrl = node_->GetComponent<AnimationController>(true);

    // Update the in air timer. Reset if grounded
    if (!m_onGround)
        m_inAirTimer += timeStep;
    else
        m_inAirTimer = 0.0f;
    // When character has been in air less than 1/10 second, it's still interpreted as being on ground
    bool softGrounded = m_inAirTimer < INAIR_THRESHOLD_TIME;

    // Update movement & animation
    const Quaternion& rot = node_->GetRotation();
    Vector3 moveDir = Vector3::ZERO;
    const Vector3& velocity = body->GetLinearVelocity();
    // Velocity on the XZ plane
    Vector3 planeVelocity(velocity.x_, 0.0f, velocity.z_);

	GetNode()->SetRotation(Quaternion(m_controls.yaw_, Vector3(0,1,0)));

    if (m_controls.IsDown(BW::CTRL_FORWARD))
	{
        moveDir += Vector3::FORWARD;
	}

    if (m_controls.IsDown(BW::CTRL_BACK))
	{
        moveDir += Vector3::BACK;
	}

    if (m_controls.IsDown(BW::CTRL_LEFT))
	{
        moveDir += Vector3::LEFT;
	}

    if (m_controls.IsDown(BW::CTRL_RIGHT))
	{
        moveDir += Vector3::RIGHT;
	}

    // Normalize move vector so that diagonal strafing is not faster
    if (moveDir.LengthSquared() > 0.0f)
        moveDir.Normalize();

    // If in air, allow control, but slower than when on ground
    body->ApplyImpulse(rot * moveDir * (softGrounded ? MOVE_FORCE : INAIR_MOVE_FORCE));

    if (softGrounded)
    {
        // When on ground, apply a braking force to limit maximum ground velocity
        Vector3 brakeForce = -planeVelocity * BRAKE_FORCE;
        body->ApplyImpulse(brakeForce);

        // Jump. Must release jump control between jumps
//        if (m_controls.IsDown(CTRL_JUMP))
//        {
//            if (m_okToJump)
//            {
//                body->ApplyImpulse(Vector3::UP * JUMP_FORCE);
//                m_okToJump = false;
//                animCtrl->PlayExclusive("Models/Mutant/Mutant_Jump1.ani", 0, false, 0.2f);
//            }
//        }
//        else
//		{
//            m_okToJump = true;
//		}
    }

//    if ( !onGround_ )
//    {
//        animCtrl->PlayExclusive("Models/Mutant/Mutant_Jump1.ani", 0, false, 0.2f);
//    }
//    else
//    {
//        // Play walk animation if moving on ground, otherwise fade it out
//        if (softGrounded && !moveDir.Equals(Vector3::ZERO))
//            animCtrl->PlayExclusive("Models/Mutant/Mutant_Run.ani", 0, true, 0.2f);
//        else
//            animCtrl->PlayExclusive("Models/Mutant/Mutant_Idle0.ani", 0, true, 0.2f);
//
//        // Set walk animation speed proportional to velocity
//        animCtrl->SetSpeed("Models/Mutant/Mutant_Run.ani", planeVelocity.Length() * 0.3f);
//    }

    // Reset grounded flag for next frame
    m_onGround = false;
}

void BW::Character::HandleNodeCollision(StringHash eventType, VariantMap& eventData)
{
	// Check collision contacts and see if character is standing on ground (look for a contact that has near vertical normal)
    using namespace NodeCollision;

    MemoryBuffer contacts(eventData[P_CONTACTS].GetBuffer());

    while (!contacts.IsEof())
    {
        Vector3 contactPosition = contacts.ReadVector3();
        Vector3 contactNormal = contacts.ReadVector3();
        /*float contactDistance = */contacts.ReadFloat();
        /*float contactImpulse = */contacts.ReadFloat();

        // If contact is below node center and pointing up, assume it's a ground contact
        if (contactPosition.y_ < (node_->GetPosition().y_ + 1.0f))
        {
            float level = contactNormal.y_;
            if (level > 0.75)
                m_onGround = true;
        }
    }
}
