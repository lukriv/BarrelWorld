#ifndef __GAME_RENDER_RIGID_BODY_H__
#define __GAME_RENDER_RIGID_BODY_H__

#include "grenderposition.h"
#include "grenderobject.h"
#include <GameSystem/gpropery.h>

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

	GameErrorCode Create(GamePropertyContainer &propMap );
	
	void Destroy();	
	
    GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);
	
	/**
	 * \brief Process update if it is necessary
	 */
	virtual void Update() override;
	
};

#endif // __GAME_RENDER_RIGID_BODY_H__
