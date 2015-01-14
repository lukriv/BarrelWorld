#include "gphyscmgr.h"
#include <GameResHold/gdeftables.h>
#include <GameComp/gentity.h>
#include <GameComp/TransformComp/gtranscomp.h>
#include "gphyscomp.h"

btCollisionShape * PhysicsCompManager::CreateCollisionShape(PhysShapeDef &physShapeDef)
{
	btCollisionShape *pColShape = nullptr;
	switch(physShapeDef.m_shapeType)
	{
		case PhysShapeDef::SHAPE_TYPE_BOX:
		{
			pColShape = new btBoxShape(btVector3(physShapeDef.m_boxHalfSize.x
												, physShapeDef.m_boxHalfSize.y
												, physShapeDef.m_boxHalfSize.z));
			break;
		}
		default:
			FWGLOG_ERROR(wxT("Unknown shape type"), m_spLogger);
			break;
	}
	
	return pColShape;
}

PhysicsCompManager::PhysicsCompManager(GameLogger* pLogger) : m_pSolver(nullptr)
									, m_pBroadphase(nullptr)
									, m_pCollisionConfig(nullptr)
									, m_pDispatcher(nullptr)
									, m_pDynamicsWorld(nullptr)
									, m_spLogger(pLogger){}


PhysicsCompManager::~PhysicsCompManager()
{
	Uninitialize();
}

GameErrorCode PhysicsCompManager::Initialize()
{
	m_pSolver = new btSequentialImpulseConstraintSolver;
	if(m_pSolver == nullptr)
	{
		Uninitialize();
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}	
	
	m_pBroadphase = new btDbvtBroadphase();
	if(m_pBroadphase == nullptr)
	{
		Uninitialize();
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_pCollisionConfig = new btDefaultCollisionConfiguration();
	if(m_pCollisionConfig == nullptr)
	{
		Uninitialize();
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfig);
	if(m_pDispatcher == nullptr)
	{
		Uninitialize();
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_pDynamicsWorld =  new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfig);
	if(m_pDynamicsWorld == nullptr)
	{
		Uninitialize();
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	return FWG_NO_ERROR;
}


void PhysicsCompManager::Uninitialize()
{
	if(m_pDynamicsWorld)
	{
		delete m_pDynamicsWorld;
		m_pDynamicsWorld = nullptr;
	}
	
	if(m_pDispatcher)
	{
		delete m_pDispatcher;
		m_pDispatcher = nullptr;
	}
	
	if(m_pCollisionConfig)
	{
		delete m_pCollisionConfig;
		m_pCollisionConfig = nullptr;
	}
	
	if(m_pBroadphase)
	{
		delete m_pBroadphase;
		m_pBroadphase = nullptr;
	}
	
	if(m_pSolver)
	{
		delete m_pSolver;
		m_pSolver = nullptr;
	}
}

GameErrorCode PhysicsCompManager::CreatePhysicsComponent(PhysCompDef& physDef, GameEntity* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;
	if((pEntity == nullptr)||(pEntity->GetComponent(GAME_COMP_TRANSFORM) == nullptr))
	{
		FWGLOG_ERROR(wxT("Entity or its transform component is null"), m_spLogger);
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	if(physDef.m_shape.IsEmpty())
	{
		FWGLOG_ERROR(wxT("Physics definition has empty physics shape"), m_spLogger);
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	RefObjSmPtr<PhysicsComponent> spPhysComp;
	FWG_RETURN_FAIL(GameNewChecked(spPhysComp.OutRef(), this));
	
	btCollisionShape *pColShape = CreateCollisionShape(*physDef.m_shape);
	if(pColShape == nullptr)
	{
		FWGLOG_ERROR(wxT("Physics definition has empty physics shape"), m_spLogger);
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	TransformComponent *pTrans = reinterpret_cast<TransformComponent*>(pEntity->GetComponent(GAME_COMP_TRANSFORM));
	btScalar mass = physDef.m_mass;
	btVector3 localInertia(physDef.m_inertiaVector.x, physDef.m_inertiaVector.y, physDef.m_inertiaVector.z);
	
	pColShape->calculateLocalInertia(mass, localInertia);
	
	btRigidBody::btRigidBodyConstructionInfo info(mass, pTrans, pColShape, localInertia);
	
	btRigidBody *body = new btRigidBody(info);
	
	if(FWG_FAILED(result = spPhysComp->Initialize(pEntity, body)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Physics component initialize failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;		
	}
	
	if(FWG_FAILED(result = pEntity->AddComponent(spPhysComp)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Add physics component to entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;		
	}

	return result;
	
}

GameErrorCode PhysicsCompManager::ProcessPhysics()
{
	m_pDynamicsWorld->stepSimulation(1.0f/60.0f);
	return FWG_NO_ERROR;
}
