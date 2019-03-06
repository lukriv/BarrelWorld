#include "EntityCreator.h"

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/VertexBuffer.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/IO/Log.h>
#include "Character.h"
#include "LayerDefs.h"
#include "DebugTools.h"

using namespace Urho3D;


Urho3D::Node* BW::EntityCreator::CreatePistol(const Urho3D::String& name, Urho3D::Application* pApp, Urho3D::Node* pParent)
{
	ResourceCache* cache= pApp->GetSubsystem<ResourceCache>();
	Node* pPistolNode = pParent->CreateChild(name);
    
    StaticModel* pAvatarObject=pPistolNode->CreateComponent<StaticModel>();
    pAvatarObject->SetModel(cache->GetResource<Model>("Objects/items/pistol/Models/Pistol.mdl"));
    pAvatarObject->SetMaterial(cache->GetResource<Material>("Objects/items/pistol/Materials/PistolMat.xml"));
	pAvatarObject->SetCastShadows(false);
	
	// Create rigidbody, and set non-zero mass so that the body becomes dynamic
    //RigidBody* body = pPistolNode->CreateComponent<RigidBody>();
    //body->SetCollisionLayer(BW::LAYER_AVATARS);
    //body->SetMass(70.0f);

    // Set zero angular factor so that physics doesn't turn the character on its own.
    // Instead we will control the character yaw manually
    //body->SetAngularFactor(Vector3::ZERO);

    // Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
    //body->SetCollisionEventMode(COLLISION_ALWAYS);

    // Set a capsule shape for collision
    // CollisionShape* shape = pPistolNode->CreateComponent<CollisionShape>();
    // shape->SetCapsule(1.0f, 1.5f, Vector3(0.0f, 0.0f, 0.0f));
	
	//pPistolNode->CreateComponent<BW::Character>();
	
	
	
	return pPistolNode;
}

Node* BW::EntityCreator::CreateAvatar(const String &name, Application* pApp, Node* pScene)
{
	ResourceCache* cache= pApp->GetSubsystem<ResourceCache>();
	Node* pAvatarNode = pScene->CreateChild(name);
    
    StaticModel* pAvatarObject=pAvatarNode->CreateComponent<StaticModel>();
    pAvatarObject->SetModel(cache->GetResource<Model>("Objects/BarrelAvatar/Models/Barrel.mdl"));
    pAvatarObject->SetMaterial(cache->GetResource<Material>("Objects/BarrelAvatar/Materials/BasicMat.xml"));
	pAvatarObject->SetCastShadows(true);
	
	// Create rigidbody, and set non-zero mass so that the body becomes dynamic
    RigidBody* body = pAvatarNode->CreateComponent<RigidBody>();
    body->SetCollisionLayer(BW::LAYER_AVATARS);
    body->SetMass(70.0f);

    // Set zero angular factor so that physics doesn't turn the character on its own.
    // Instead we will control the character yaw manually
    body->SetAngularFactor(Vector3::ZERO);

    // Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
    body->SetCollisionEventMode(COLLISION_ALWAYS);

    // Set a capsule shape for collision
    CollisionShape* shape = pAvatarNode->CreateComponent<CollisionShape>();
    shape->SetCapsule(1.0f, 1.5f, Vector3(0.0f, 0.0f, 0.0f));
	
	pAvatarNode->CreateComponent<BW::Character>();
	
	Node* pPistol = CreatePistol("Pistol", pApp, pScene);
	
	pPistol->SetParent(pAvatarNode);
	pPistol->Translate(Vector3(0,0,0.8));
	
	return pAvatarNode;
}


Node* BW::EntityCreator::CreateAvatar2(const String &name, Application* pApp, Node* pScene)
{
	ResourceCache* cache= pApp->GetSubsystem<ResourceCache>();
	Node* pAvatarNode = pScene->CreateChild(name);
    
    AnimatedModel* pAvatarObject=pAvatarNode->CreateComponent<AnimatedModel>();
    BW::DebugTools::LogModel(cache->GetResource<Model>("Models/Skeleton.mdl"));
	pAvatarObject->SetModel(cache->GetResource<Model>("Models/Skeleton.mdl"));
    pAvatarObject->SetMaterial(cache->GetResource<Material>("Materials/BaseMat.xml"));
	pAvatarObject->SetCastShadows(true);
	

	
	pAvatarNode->CreateComponent<AnimationController>();
	
	// Create rigidbody, and set non-zero mass so that the body becomes dynamic
    RigidBody* body = pAvatarNode->CreateComponent<RigidBody>();
    body->SetCollisionLayer(BW::LAYER_AVATARS);
    body->SetMass(70.0f);

    // Set zero angular factor so that physics doesn't turn the character on its own.
    // Instead we will control the character yaw manually
    body->SetAngularFactor(Vector3::ZERO);

    // Set the rigidbody to signal collision also when in rest, so that we get ground collisions properly
    body->SetCollisionEventMode(COLLISION_ALWAYS);

    // Set a capsule shape for collision
    CollisionShape* shape = pAvatarNode->CreateComponent<CollisionShape>();
    shape->SetCapsule(1.0f, 2.0f, Vector3(0.0f, 0.0f, 0.0f));
	
	pAvatarNode->CreateComponent<BW::Character>();
	
	return pAvatarNode;
}





Urho3D::Node* BW::EntityCreator::CreatePlanet(const Urho3D::String& name, Urho3D::Application* pApp, Urho3D::Node* pScene)
{
	ResourceCache* cache= pApp->GetSubsystem<ResourceCache>();
	Node* pPlanetNode = pScene->CreateChild(name);
}
