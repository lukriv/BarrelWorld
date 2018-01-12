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


Node* BW::EntityCreator::CreateAvatar2(const String &name, Application* pApp, Scene* pScene)
{
	ResourceCache* cache= pApp->GetSubsystem<ResourceCache>();
	Node* pAvatarNode = pScene->CreateChild(name);
    
    AnimatedModel* pAvatarObject=pAvatarNode->CreateComponent<AnimatedModel>();
    LogModel(cache->GetResource<Model>("Models/Skeleton.mdl"));
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

void BW::EntityCreator::LogModel(Urho3D::Model* pModel)
{
	String str;
	str.AppendWithFormat("Model: %s", pModel->GetName());
	LogModelGeometry(pModel);
	LogModelSkeleton(pModel);
}

void BW::EntityCreator::LogModelGeometry(Urho3D::Model* pModel)
{
	//Vector<Vector<SharedPtr<Geometry> > >
	auto geomVec = pModel->GetGeometries();
	int i = 0, j = 0;
	for(auto &geom : geomVec )
	{
		for(auto &geomPtr: geom)
		{
			String str;
			
			str.AppendWithFormat("Geometry %d, %d", i, j);
			Log::Write(LOG_INFO, str);
			str.Clear();
			
			str.AppendWithFormat(" Number of vertex buffers %u", geomPtr->GetNumVertexBuffers());
			Log::Write(LOG_INFO, str);
			str.Clear();
			
			//const Vector< SharedPtr< VertexBuffer > >
			auto vertexBuffVec = geomPtr->GetVertexBuffers();
			
			for(auto &vertexBuff : vertexBuffVec)
			{
				str.AppendWithFormat("  Vertex count: %u, vertex size %u", vertexBuff->GetVertexCount(), vertexBuff->GetVertexSize());
				Log::Write(LOG_INFO, str);
				str.Clear();
				
				unsigned char* data = reinterpret_cast<unsigned char*>(vertexBuff->Lock(0, vertexBuff->GetVertexCount()));
				unsigned char* dataPtr = data;
				
				str.AppendWithFormat("  Verteces:", vertexBuff->GetVertexCount(), vertexBuff->GetVertexSize());
				Log::Write(LOG_INFO, str);
				str.Clear();
				//const PODVector< VertexElement > &
				auto elems = vertexBuff->GetElements();
				for(unsigned int verIter = 0; verIter < vertexBuff->GetVertexCount(); ++verIter)
				{
					str.AppendWithFormat("  Vertex %u", verIter);
					Log::Write(LOG_INFO, str);
					str.Clear();
					for(auto &elem: elems)
					{
						LogVertexElement(elem, dataPtr);
					}
					dataPtr += vertexBuff->GetVertexSize();
				}
			}
			
			
			
			++j;
		}
		++i;
	}
}

void BW::EntityCreator::LogVertexElement(const Urho3D::VertexElement& vertexElement, void* data)
{
	String str;
	str.Append("   ");
	
	switch (vertexElement.semantic_)
	{
		case SEM_POSITION		: str.Append("SEM_POSITION     "); break;
		case SEM_NORMAL         : str.Append("SEM_NORMAL       "); break;
		case SEM_BINORMAL       : str.Append("SEM_BINORMAL     "); break;
		case SEM_TANGENT        : str.Append("SEM_TANGENT      "); break;
		case SEM_TEXCOORD       : str.Append("SEM_TEXCOORD     "); break;
		case SEM_COLOR          : str.Append("SEM_COLOR        "); break;
		case SEM_BLENDWEIGHTS   : str.Append("SEM_BLENDWEIGHTS "); break;
		case SEM_BLENDINDICES   : str.Append("SEM_BLENDINDICES "); break;
		case SEM_OBJECTINDEX    : str.Append("SEM_OBJECTINDEX  "); break;
		default:
			break;
	}
	
	switch (vertexElement.type_)
	{
		case TYPE_INT: 
			str.Append("TYPE_INT [");
			str.AppendWithFormat("%d", *(reinterpret_cast<int*>(data)));
			break;
        case TYPE_FLOAT: 
			str.Append("TYPE_FLOAT [");
			str.AppendWithFormat("%f", *(reinterpret_cast<float*>(data)));
			break;
        case TYPE_VECTOR2: 
		{
			str.Append("TYPE_VECTOR2 [");
			Vector2 *vec2 = reinterpret_cast<Vector2*>(data);
			str.AppendWithFormat("%f, %f", vec2->x_, vec2->y_ );
		}
			break;
        case TYPE_VECTOR3: 
		{
			str.Append("TYPE_VECTOR3 [");
			Vector3 *vec3 = reinterpret_cast<Vector3*>(data);
			str.AppendWithFormat("%f, %f, %f", vec3->x_, vec3->y_, vec3->z_ );
		}
			break;
        case TYPE_VECTOR4: 
		{
			str.Append("TYPE_VECTOR4 [");
			Vector4 *vec4 = reinterpret_cast<Vector4*>(data);
			str.AppendWithFormat("%f, %f, %f, %f", vec4->x_, vec4->y_, vec4->z_, vec4->w_);
		}
			break;
        case TYPE_UBYTE4: 
		{
			str.Append("TYPE_UBYTE4 [");
			unsigned char *vec = reinterpret_cast<unsigned char*>(data);
			str.AppendWithFormat("%u, %u, %u, %u", vec[0], vec[1], vec[2], vec[3]);
		}
			break;
        case TYPE_UBYTE4_NORM: 
			str.Append("TYPE_UBYTE4_NORM [");
			break;
		default:
			break;
	}
	
	str.Append("]");
	
	Log::Write(LOG_INFO, str);
	
}



void BW::EntityCreator::LogModelSkeleton(Urho3D::Model* pModel)
{
	Skeleton &skeleton = pModel->GetSkeleton();
	
	String str;
	str.AppendWithFormat("Skeleton ( %u bones )", skeleton.GetNumBones());
	Log::Write(LOG_INFO, str);
	str.Clear();
	
	const Vector< Bone > & bonesVec = skeleton.GetBones();
	int i = 0;
	for(const Bone &bone : bonesVec)
	{
		str.AppendWithFormat(" Bone %d", i);
		Log::Write(LOG_INFO, str);
		str.Clear();
		
//		if(!bone.name_.Empty())
//		{
//			str.AppendWithFormat("  name: %s", bone.name_);
//			Log::Write(LOG_INFO, str);
//			str.Clear();
//		}
		
		str.AppendWithFormat("  nameHash: %u", bone.nameHash_.Value());
		Log::Write(LOG_INFO, str);
		str.Clear();		
		
		str.AppendWithFormat("  parentIndex: %u", bone.parentIndex_);
		Log::Write(LOG_INFO, str);
		str.Clear();
		++i;
	}
	

}
