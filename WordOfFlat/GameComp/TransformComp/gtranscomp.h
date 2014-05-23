#ifndef __GAME_TRANSFORM_COMPONENT_H__
#define __GAME_TRANSFORM_COMPONENT_H__

#include "OGRE/OgreSceneNode.h"
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include "../RenderComp/grendercomp.h"

class TransformComponent : public RefObjectImpl<IRefObject> {
	Ogre::SceneNode *m_pSceneNode;
public:
	TransformComponent() : m_pSceneNode(NULL) {}
				
	inline void SetSceneNode(Ogre::SceneNode *pSceneNode) { m_pSceneNode = pSceneNode; }
	
	
	
	inline Ogre::SceneNode* GetSceneNode() { return m_pSceneNode; }
	
	void Connect(RenderComponent &renderComp);
	
	
};


#endif //__GAME_TRANSFORM_COMPONENT_MANAGER_H__