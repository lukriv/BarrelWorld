#include "gtranscomp.h"

#include <bullet/LinearMath/btAlignedAllocator.h>

typedef btAlignedAllocator< TransformData, 16 > TAllocator;

TransformComponent::TransformComponent() : ComponentBase(GAME_COMP_TRANSFORM)
										, m_pTransData(nullptr)
										, m_pParent(nullptr)
{}

GameErrorCode TransformComponent::Initialize()
{
	TAllocator allocator;
	//m_pTransData = allocator.allocate(1);
	FWG_RETURN_FAIL(GameNewChecked(m_pTransData));
	if(m_pTransData == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	m_pTransData->m_translate = Vectormath::Aos::Vector3(0.0f, 0.0f, 0.0f);
	m_pTransData->m_scale = Vectormath::Aos::Vector3(1.0f, 1.0f, 1.0f);
	m_pTransData->m_rotation.identity();
	
	return FWG_NO_ERROR;
}

TransformComponent::~TransformComponent()
{
	TAllocator allocator;
	if(m_pTransData != nullptr)
	{
		//allocator.deallocate(m_pTransData);
		GameDelete(m_pTransData);
		m_pTransData = nullptr;
	}
}

GameErrorCode TransformComponent::ReceiveMessage(TaskMessage& )
{
	return FWG_NO_ERROR;
}

GameErrorCode TransformComponent::ReinitComponent(GameEntity* pNewParentEntity)
{
	//todo: find physics and connect to it (and find render??)
	m_pParent = pNewParentEntity;
	return FWG_NO_ERROR;
}

GameErrorCode TransformComponent::Update()
{
	// no update is necessary
	return FWG_NO_ERROR;
}
