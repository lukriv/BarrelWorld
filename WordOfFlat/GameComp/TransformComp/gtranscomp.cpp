#include "gtranscomp.h"

#include <bullet/LinearMath/btAlignedAllocator.h>
#include <GameComp/gentity.h>

typedef btAlignedAllocator< TransformData, 16 > TAllocator;

TransformComponent::TransformComponent() : ComponentBase(GAME_COMP_TRANSFORM)
										, m_pTransData(nullptr)
										, m_pParent(nullptr)
{}

GameErrorCode TransformComponent::Initialize(GameEntity *pEntity)
{
	if(pEntity == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	TAllocator allocator;
	m_pTransData = allocator.allocate(1);
	//FWG_RETURN_FAIL(GameNewChecked(m_pTransData));
	if(m_pTransData == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	m_pTransData->m_translate = btVector3(0.0f, 0.0f, 0.0f);
	m_pTransData->m_scale = btVector3(1.0f, 1.0f, 1.0f);
	m_pTransData->m_rotation = btQuaternion::getIdentity();
	
	// set parent
	m_pParent = pEntity;
	
	return FWG_NO_ERROR;
}

TransformComponent::~TransformComponent()
{
	TAllocator allocator;
	if(m_pTransData != nullptr)
	{
		allocator.deallocate(m_pTransData);
		//GameDelete(m_pTransData);
		m_pTransData = nullptr;
	}
}

GameErrorCode TransformComponent::ReceiveMessage(TaskMessage& )
{
	return FWG_NO_ERROR;
}

GameErrorCode TransformComponent::ReinitComponent(GameEntity* pNewParentEntity)
{
	//connection to physics does the physics component itself
	m_pParent = pNewParentEntity;
	return FWG_NO_ERROR;
}

GameErrorCode TransformComponent::Update()
{
	// no update is necessary
	return FWG_NO_ERROR;
}

void TransformComponent::getWorldTransform(btTransform& worldTrans) const
{
	if(m_pTransData)
	{
		btVector3 transform(m_pTransData->m_translate);
		btQuaternion rotation(m_pTransData->m_rotation);
							
		worldTrans.setOrigin(transform);
		worldTrans.setRotation(rotation);
	}
}

void TransformComponent::setWorldTransform(const btTransform& worldTrans)
{
	m_pTransData->m_rotation = worldTrans.getRotation();
	m_pTransData->m_translate = worldTrans.getOrigin();
	
	//transform was updated
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	m_pParent->ReceiveMessage(task, (~GAME_COMP_MASK_PHYSICS));
		
}

