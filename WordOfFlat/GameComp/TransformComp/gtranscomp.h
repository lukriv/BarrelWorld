#ifndef __GAME_TRANSFORM_COMPONENT_H__
#define __GAME_TRANSFORM_COMPONENT_H__

#include <bullet/vectormath/vmInclude.h>
#include <bullet/LinearMath/btMotionState.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameComp/PhysicsComp/gogrebulletutils.h>
#include "../gcompbase.h"

ATTRIBUTE_ALIGNED16(struct) TransformData {
public:
	btVector3 		m_translate;
	btVector3		m_scale;
	btQuaternion	m_rotation;
};


class GameEntity;

class TransformComponent : public ComponentBase, public btMotionState {
	TransformData *m_pTransData;
	GameEntity* m_pParent;
	
public:
	TransformComponent();
	~TransformComponent();
	
	GameErrorCode Initialize(GameEntity *pEntity);
	
	inline TransformData* GetData() { return m_pTransData; }
	
	inline const Ogre::Quaternion GetOgreRotation();
	inline const Ogre::Vector3 GetOgreTranlate();
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	virtual GameErrorCode ReinitComponent(GameEntity* pNewParentEntity);
	virtual GameErrorCode Update();
	
	virtual void getWorldTransform(btTransform& worldTrans) const override;
	virtual void setWorldTransform(const btTransform& worldTrans) override;
	
};


const Ogre::Quaternion TransformComponent::GetOgreRotation()
{
	return cvt(m_pTransData->m_rotation);
}

const Ogre::Vector3 TransformComponent::GetOgreTranlate()
{
	return cvt(m_pTransData->m_translate);
}


#endif //__GAME_TRANSFORM_COMPONENT_MANAGER_H__