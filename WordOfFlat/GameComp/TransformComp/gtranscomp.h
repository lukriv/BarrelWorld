#ifndef __GAME_TRANSFORM_COMPONENT_H__
#define __GAME_TRANSFORM_COMPONENT_H__

#include <OGRE/OgreSceneNode.h>
#include <bullet/vectormath/vmInclude.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include "../RenderComp/grendercomp.h"

class TransformComponent : public RefObjectImpl<IRefObject> {

	Vectormath::Aos::Vector3 	m_translate ATTRIBUTE_ALIGNED16;
	Vectormath::Aos::Vector3	m_scale ATTRIBUTE_ALIGNED16;
	Vectormath::Aos::Quat		m_rotation ATTRIBUTE_ALIGNED16;
public:
	TransformComponent() : m_pSceneNode(NULL) {}
				
	inline void SetSceneNode(Ogre::SceneNode *pSceneNode) { m_pSceneNode = pSceneNode; }
	
	inline Ogre::SceneNode* GetSceneNode() { return m_pSceneNode; }
	
	void Connect(RenderComponent &renderComp);
	
	inline const Vectormath::Aos::Vector3& GetTranslate() const { return m_translate; }
	
	inline const Vectormath::Aos::Vector3& GetScale() const { return m_scale; }
	
	inline const Vectormath::Aos::Quat&	GetRotation() const { return m_rotation; }
	
	
};


#endif //__GAME_TRANSFORM_COMPONENT_MANAGER_H__