#include "grendercamera.h"


RenderCamera::RenderCamera(RenderCompManager* pCompManager)
{
}

RenderCamera::~RenderCamera()
{
}

GameErrorCode RenderCamera::Initialize(TransformComponent* pTransform)
{
	GameErrorCode result = FWG_NO_ERROR;
    m_spTransform = pTransform;
    if(m_spTransform.IsEmpty()) 
	{
		// transform component cannot be null - add transform component to entity at first
		return FWG_E_INVALID_PARAMETER_ERROR;
    }

    TransformData* transData = m_spTransform->GetData();

	if(FWG_FAILED(result = RenderComponent::Initialize( Ogre::Vector3(transData->m_translate.getX(), transData->m_translate.getY(), transData->m_translate.getZ())
		, Ogre::Quaternion(transData->m_rotation.getW(), transData->m_rotation.getX(), transData->m_rotation.getY(), transData->m_rotation.getZ()))))
	{
		return result;	
	}
	


	return FWG_NO_ERROR;
}

void RenderCamera::Clear()
{
}

GameErrorCode RenderCamera::Create(const wxString& cameraName)
{
}

Ogre::Camera* RenderCamera::GetOgreCamera()
{
}



GameErrorCode RenderCamera::Load(wxXmlNode* XMLNode)
{
	// todo: refactor this loader
	
	// Process attributes
    wxString name = XMLNode->GetAttribute(wxT("name"));
    wxString id = XMLNode->GetAttribute(wxT("id"));
    Ogre::Real fov = GetAttribReal(XMLNode, wxT("fov"), 45);
    Ogre::Real aspectRatio = GetAttribReal(XMLNode, wxT("aspectRatio"), 1.3333);
    wxString projectionType = XMLNode->GetAttribute(wxT("projectionType"), wxT("perspective"));
 
    // Create the camera
    Ogre::Camera *pCamera = m_sceneMgr->createCamera(name.ToStdString());
    if(pParent)
	{
        pParent->attachObject(pCamera);
	}
 
    // Set the field-of-view
    //! @todo Is this always in degrees?
    pCamera->setFOVy(Ogre::Degree(fov));
 
    // Set the aspect ratio
    pCamera->setAspectRatio(aspectRatio);
 
    // Set the projection type
    if(projectionType == wxT("perspective"))
	{
        pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
    } else if(projectionType == wxT("orthographic")) {
        pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	}
 
    wxXmlNode *pElement = XMLNode->GetChildren();
 
	while(pElement)
	{
 
		// Process clipping (?)
		if(pElement->GetName() == wxT("clipping"))
		{
			Ogre::Real nearDist = GetAttribReal(pElement, wxT("near"));
			pCamera->setNearClipDistance(nearDist);
	 
			Ogre::Real farDist =  GetAttribReal(pElement, wxT("far"), 50000.0);
			pCamera->setFarClipDistance(farDist);
		
		// Process position (?)
		} else if(pElement->GetName() == wxT("position")) {
			pCamera->setPosition(ParseVector3(pElement));
	 
		// Process rotation (?)
		} else if(pElement->GetName() == wxT("rotation")) {
			pCamera->setOrientation(ParseQuaternion(pElement));
	 
		} else {
			ProcessUnknownTag(pElement);
		}
		
		pElement = pElement->GetNext();	
	}
	
	return FWG_NO_ERROR;
}

void RenderCamera::ProcessUpdate()
{
}



GameErrorCode RenderCamera::Store(wxXmlNode* ParentNode)
{
}


