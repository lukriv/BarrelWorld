#ifndef __GAME_TRANSFORM_COMPONENT_H__
#define __GAME_TRANSFORM_COMPONENT_H__

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

class wxXmlNode;

class TransformComponent : public ComponentBase, public btMotionState {
private:
	TransformData *m_pTransData;
public:
	TransformComponent();
	~TransformComponent();
	
	GameErrorCode Initialize();
	
	inline TransformData* GetData() { return m_pTransData; }
	
	inline const Ogre::Quaternion GetOgreRotation();
	inline const Ogre::Vector3 GetOgreTranslate();
	
	inline void SetOgreRotation(const Ogre::Quaternion& rot);
	inline void SetOgreTranslate(const Ogre::Vector3& trans);
	
	virtual GameErrorCode ReceiveMessage(TaskMessage& msg);
	
	virtual void getWorldTransform(btTransform& worldTrans) const override;
	virtual void setWorldTransform(const btTransform& worldTrans) override;
	
	GameErrorCode Create( const btVector3& position, const btVector3& scale, const btQuaternion& rotation);
	
	GameErrorCode Load(wxXmlNode *XMLNode);
	GameErrorCode Store(wxXmlNode *ParentNode);
	
};


const Ogre::Quaternion TransformComponent::GetOgreRotation()
{
	return cvt(m_pTransData->m_rotation);
}

const Ogre::Vector3 TransformComponent::GetOgreTranslate()
{
	return cvt(m_pTransData->m_translate);
}

void TransformComponent::SetOgreRotation(const Ogre::Quaternion& rot)
{
	m_pTransData->m_rotation = cvt(rot);
}

void TransformComponent::SetOgreTranslate(const Ogre::Vector3& trans)
{
	m_pTransData->m_translate = cvt(trans);
}


#endif //__GAME_TRANSFORM_COMPONENT_MANAGER_H__