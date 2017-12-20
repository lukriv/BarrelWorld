#include "EntityCreator.h"

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>

using namespace Urho3D;


Node* BW::EntityCreator::CreateAvatar(const String &name, Application* pApp, Scene* pScene)
{
	ResourceCache* cache= pApp->GetSubsystem<ResourceCache>();
	Node* pAvatarNode = pScene->CreateChild(name);
    
    StaticModel* pAvatarObject=pAvatarNode->CreateComponent<StaticModel>();
    pAvatarObject->SetModel(cache->GetResource<Model>("Models/Avatar.mdl"));
    pAvatarObject->SetMaterial(cache->GetResource<Material>("Materials/AvatarMaterial.xml"));
	pAvatarObject->SetCastShadows(true);
	
	// Create rigidbody, and set non-zero mass so that the body becomes dynamic
    RigidBody* body = pAvatarNode->CreateComponent<RigidBody>();
    body->SetCollisionLayer(1);
    body->SetMass(70.0f);

    // Set zero angular factor so that physics doesn't turn the character on its own.
    // Instead we will control the character yaw manually
    body->SetAngularFactor(Vector3::ZERO);

    // Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
    body->SetCollisionEventMode(COLLISION_ALWAYS);

    // Set a capsule shape for collision
    CollisionShape* shape = pAvatarNode->CreateComponent<CollisionShape>();
    shape->SetCapsule(1.0f, 2.0f, Vector3(0.0f, 0.0f, 0.0f));
	
	return pAvatarNode;
}
