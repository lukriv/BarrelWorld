#ifndef __GAME_RENDER_RIGID_BODY_H__
#define __GAME_RENDER_RIGID_BODY_H__

#include "grenderposition.h"
#include "grenderobject.h"

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
class RenderRigidBody : public RenderObject 
{

	Ogre::Entity *m_pEntity;
	
public:

    RenderRigidBody(GameRenderSystem* pCompManager);
    ~RenderRigidBody();

	GameErrorCode Create(const wxString& meshName, const wxString& materialName );
	
	void Destroy();	
	
    GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);
	
	/**
	 * \brief Process update if it is necessary
	 */
	virtual void Update() override;
	
	/**
	 * @brief Called in asynchronous component creation
	 * @param pContext
	 */
	virtual void OnCreation(void *pContext) override;
	
};

#endif // __GAME_RENDER_RIGID_BODY_H__
