#ifndef __GAME_RENDER_CAMERA_H__
#define __GAME_RENDER_CAMERA_H__

#include "grendercomp.h"

class RenderMoveable;

/**
 * \brief Render generic object reads generic dotScene format and display it
 *
 * This object can handle only transforms messages. Due to generic content it does not know what is inside.
 */
class RenderCamera : public RenderComponent
{

    RefObjSmPtr<RenderMoveable> m_spMoveable;
	Ogre::Camera *m_pCamera;
	
public:

    RenderCamera(RenderCompManager* pCompManager);
    ~RenderCamera();

    GameErrorCode Initialize(RenderMoveable* pRenderMoveable);
	
	GameErrorCode Create(const wxString& cameraName);
	
	Ogre::Camera* GetOgreCamera();
	
    GameErrorCode Load(wxXmlNode* XMLNode);
    GameErrorCode Store(wxXmlNode* ParentNode);
	
	virtual void ProcessUpdate() override;
};

#endif // __GAME_RENDER_CAMERA_H__