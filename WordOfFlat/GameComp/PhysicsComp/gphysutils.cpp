#include "gphysutils.h"
#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>

static const btScalar VECTOR_SIZE = 3000;

btScalar GamePhysicsUtils::ComputeGroundDistance(GamePhysicsSystem& physSystem, PhysicsBase& physComponent)
{
	btVector3 origin(physComponent.GetRigidBody()->getWorldTransform().getOrigin());
	btVector3 end(origin.getX(), -VECTOR_SIZE, origin.getZ());
	btVector3 min;
	
	btCollisionWorld::ClosestRayResultCallback callback(origin, end);
	
	physSystem.GetDynamicsWorld()->rayTest(origin, end, callback);
	
	physComponent.GetRigidBody()->getAabb(min, end);
	
	if(callback.hasHit())
	{
		return (callback.m_closestHitFraction * VECTOR_SIZE) - (origin.getY() - min.getY());
	}
		
	return 0.0f;
}
