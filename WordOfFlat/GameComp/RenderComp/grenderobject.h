#ifndef __GAME_RENDER_OBJECT_H__
#define __GAME_RENDER_OBJECT_H__

#include "grendercompbase.h"
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
class RenderObject : public RenderComponentBase
{
protected:
    RefObjSmPtr<RenderPosition> m_spPosition;
	
public:

    RenderObject(GameRenderSystem* pCompManager);
    ~RenderObject();

    GameErrorCode Initialize(RenderPosition* pRenderMoveable);
	
};

#endif // __GAME_RENDER_OBJECT_H__