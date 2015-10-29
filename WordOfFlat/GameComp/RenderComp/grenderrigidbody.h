#ifndef __GAME_RENDER_RIGID_BODY_H__
#define __GAME_RENDER_RIGID_BODY_H__

#include "grendercomp.h"
#include "grenderposition.h"

/**
 * @class RenderRigidBody
 * @author Lukas
 * @date 04/20/15
 * @file grendertestbox.h
 * @brief Render rigid body component contains one mesh with one material
 *
 * This component is intended to use as boxes, stones, and other movable or static objects.
 * Can be combined with physics components
 *
 */
class RenderRigidBody : public RenderComponent
{

    RefObjSmPtr<RenderMoveable> m_spMoveable;
	Ogre::Entity *m_pEntity;
	
public:

    RenderRigidBody(RenderCompManager* pCompManager);
    ~RenderRigidBody();

    GameErrorCode Initialize(RenderMoveable* pRenderMoveable);
	
	GameErrorCode Create(const wxString& meshName, const wxString& materialName );
	
	void Clear();	
	
    GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);
	
	virtual void ProcessUpdate() override;
};

#endif // __GAME_RENDER_RIGID_BODY_H__
