#include "gtranscomp.h"

#include <bullet/LinearMath/btAlignedAllocator.h>

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
	m_pTransData->m_translate = Vectormath::Aos::Vector3(0.0f, 0.0f, 0.0f);
	m_pTransData->m_scale = Vectormath::Aos::Vector3(1.0f, 1.0f, 1.0f);
	m_pTransData->m_rotation.identity();
	
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
		btVector3 transform(m_pTransData->m_translate.getX()
							, m_pTransData->m_translate.getY()
							, m_pTransData->m_translate.getZ());
		btQuaternion rotation(m_pTransData->m_rotation.getX()
							, m_pTransData->m_rotation.getY()
							, m_pTransData->m_rotation.getZ()
							, m_pTransData->m_rotation.getW());
							
		worldTrans.setOrigin(transform);
		worldTrans.setRotation(rotation);
	}
}

void TransformComponent::setWorldTransform(const btTransform& worldTrans)
{
	btQuaternion rotation = worldTrans.getRotation();
	btVector3 translation = worldTrans.getOrigin();
	
	m_pTransData->m_translate.setX(translation.getX());
	m_pTransData->m_translate.setY(translation.getY());
	m_pTransData->m_translate.setZ(translation.getZ());
	
	m_pTransData->m_rotation.setX(rotation.getX());
	m_pTransData->m_rotation.setY(rotation.getY());
	m_pTransData->m_rotation.setZ(rotation.getZ());
	m_pTransData->m_rotation.setW(rotation.getW());
}

