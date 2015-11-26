#include "gphyssystem.h"
#include <GameResHold/gdeftables.h>
#include <GameComp/gentity.h>
#include <GameComp/TransformComp/gtranscomp.h>

btCollisionShape * PhysicsSystem::CreateCollisionShape(PhysShapeDef &physShapeDef)
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

PhysicsSystem::PhysicsSystem(GameLogger* pLogger) : m_pSolver(nullptr)
									, m_pBroadphase(nullptr)
									, m_pCollisionConfig(nullptr)
									, m_pDispatcher(nullptr)
									, m_pDynamicsWorld(nullptr)
									, m_spLogger(pLogger){}


PhysicsSystem::~PhysicsSystem()
{
	Uninitialize();
}

GameErrorCode PhysicsSystem::Initialize()
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
	
	btVector3 grav = m_pDynamicsWorld->getGravity();
	FWGLOG_INFO_FORMAT(wxT("gravity = %f, %f, %f"), m_spLogger, grav.getX(),grav.getY(),grav.getZ(), FWGLOG_ENDVAL);
	
	return FWG_NO_ERROR;
}


void PhysicsSystem::Uninitialize()
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

GameErrorCode PhysicsSystem::ProcessPhysics()
{
	m_pDynamicsWorld->stepSimulation(btScalar(1.)/btScalar(60.));
	return FWG_NO_ERROR;
}
