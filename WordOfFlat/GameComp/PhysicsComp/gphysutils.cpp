#include "gphysutils.h"
#include <bullet/BulletCollision/CollisionDispatch/btCollisionWorld.h>

static const btScalar VECTOR_SIZE = 3000;

btScalar GamePhysicsUtils::ComputeGroundDistance(GamePhysicsSystem& physSystem, PhysicsBase& physComponent, wxWord groupFilter, wxWord maskFilter)
{
	btVector3 origin(physComponent.GetRigidBody()->getWorldTransform().getOrigin());
	btVector3 end(origin.getX(), -VECTOR_SIZE, origin.getZ());
	btVector3 min;
	
	btCollisionWorld::ClosestRayResultCallback callback(origin, end);
	callback.m_collisionFilterGroup = groupFilter;
	callback.m_collisionFilterMask = maskFilter;
	
	physSystem.GetDynamicsWorld()->rayTest(origin, end, callback);
	
	physComponent.GetRigidBody()->getAabb(min, end);
	
	if(callback.hasHit())
	{
		return (callback.m_closestHitFraction * VECTOR_SIZE) - (origin.getY() - min.getY());
	}
		
	return 0.0f;
}

wxDword GamePhysicsUtils::ComputeTargetPoint(GamePhysicsSystem& physSystem, btVector3& origin, btVector3& point, btVector3& hitPoint)
{
	btCollisionWorld::ClosestRayResultCallback callback(origin, point);
	
	physSystem.GetDynamicsWorld()->rayTest(origin, point, callback);
	
	if(callback.hasHit())
	{
		hitPoint = callback.m_hitPointWorld;
		return static_cast<wxDword>(callback.m_collisionObject->getUserIndex());
	}
	
	return 0;
	
}
