#ifndef __GAME_RENDER_CAMERA_H__
#define __GAME_RENDER_CAMERA_H__

#include "grendercompbase.h"
#include "grenderobject.h"

class RenderPosition;

/**
 * \brief Render generic object reads generic dotScene format and display it
 *
 * This object can handle only transforms messages. Due to generic content it does not know what is inside.
 */
class RenderCamera : public RenderObject
{

	Ogre::Camera *m_pCamera;
	
public:

    RenderCamera(GameRenderSystem* pCompManager);
    ~RenderCamera();

	GameErrorCode Create(const wxString& cameraName);
	
	void Destroy();
	
	Ogre::Camera* GetOgreCamera();
	
	
	
    virtual GameErrorCode Load(wxXmlNode* XMLNode) override;
    virtual GameErrorCode Store(wxXmlNode* ParentNode) override;
	
	virtual void Update() override;
};

#endif // __GAME_RENDER_CAMERA_H__