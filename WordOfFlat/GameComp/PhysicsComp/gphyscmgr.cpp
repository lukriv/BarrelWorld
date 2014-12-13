#include "gphyscmgr.h"

PhysicsCompManager::PhysicsCompManager() : m_pSolver(nullptr)
									, m_pBroadphase(nullptr)
									, m_pCollisionConfig(nullptr)
									, m_pDispatcher(nullptr)
									, m_pDynamicsWorld(nullptr) {}


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


