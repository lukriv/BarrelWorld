#ifndef __GAME_RENDER_MOVEABLE_H__
#define __GAME_RENDER_MOVEABLE_H__

#include "grendercomp.h"


class RenderMoveable : public RenderComponent
{
protected:
    RefObjSmPtr<TransformComponent> m_spTransform;
    Ogre::SceneNode* m_pSceneNode;

public:

    RenderMoveable(RenderCompManager *pCompMgr);
    ~RenderMoveable();

    GameErrorCode Initialize(TransformComponent* pTransform);

    inline Ogre::SceneNode* GetSceneNode() { return m_pSceneNode; }
	
	virtual void ProcessUpdate();
	
	/*!
	 * \brief Destroy inner ogre object
	 */
	void Clear();
};

#endif // __GAME_RENDER_MOVEABLE_H__
