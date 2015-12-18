#ifndef __GAME_RENDER_MOVEABLE_H__
#define __GAME_RENDER_MOVEABLE_H__

#include "grendercompbase.h"

class RenderPosition : public RenderComponentBase
{
protected:
    RefObjSmPtr<TransformComponent> m_spTransform;
    Ogre::SceneNode* m_pSceneNode;

public:
    

    RenderPosition(GameRenderSystem* pCompMgr);
    ~RenderPosition();

    GameErrorCode Initialize(TransformComponent* pTransform);
	
	GameErrorCode Create();

    inline Ogre::SceneNode* GetSceneNode() { return m_pSceneNode; }

    virtual void ProcessUpdate() override;
	
	virtual void OnCreation(void* pContext) override;
	
	virtual GameErrorCode Load (wxXmlNode *pNode) override;
	virtual GameErrorCode Store (wxXmlNode *pParentNode) override;

    /*!
     * \brief Destroy inner ogre object
     */
    void Clear();
};

#endif // __GAME_RENDER_MOVEABLE_H__
