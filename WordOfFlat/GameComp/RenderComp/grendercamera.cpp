#include "grendercamera.h"

#include "grendersystem.h"
#include "grenderposition.h"
#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

RenderCamera::RenderCamera(GameRenderSystem* pCompManager) : RenderObject(pCompManager)
											, m_pCamera(nullptr)
{
}

RenderCamera::~RenderCamera()
{
	Destroy();
}

GameErrorCode RenderCamera::Create(const wxString& cameraName)
{
	m_pCamera = m_pOwnerManager->GetOgreSceneManager()->createCamera(cameraName.ToStdString());
	return FWG_NO_ERROR;
}


void RenderCamera::Destroy()
{
	if(m_pCamera)
	{
		m_pOwnerManager->GetOgreSceneManager()->destroyCamera(m_pCamera);
		m_pCamera = nullptr;
	}
}

Ogre::Camera* RenderCamera::GetOgreCamera()
{
	return m_pCamera;
}



GameErrorCode RenderCamera::Load(wxXmlNode* pNode)
{
	// todo: refactor this loader
	
	if(m_spPosition.IsEmpty())
	{
		FWGLOG_ERROR(wxT("Object is not initialized"), m_pOwnerManager->GetLogger());
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(pNode->GetName() != GAME_TAG_COMP_RENDER_CAMERA)
	{
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	wxString temp;
	float tempFloat = 0.0;
	
	wxString name;
	
	if(!pNode->GetAttribute(GAME_TAG_ATTR_NAME, &name))
	{
		FWGLOG_ERROR_FORMAT(wxT("Camera component on line %d does not contain '%s' attribute"), m_pOwnerManager->GetLogger()
													, pNode->GetLineNumber()
													, GAME_TAG_ATTR_NAME
													, FWGLOG_ENDVAL);
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
    wxString projectionType = GAME_TAG_VALUE_PERSPECTIVE;
 
    // Create the camera
    m_pCamera = m_pOwnerManager->GetOgreSceneManager()->createCamera(name.ToStdString());
    
	// attach camera to the node
	m_spPosition->GetSceneNode()->attachObject(m_pCamera);

 
    wxXmlNode *pElement = pNode->GetChildren();
 
	while(pElement)
	{
 
		// Process clipping (?)
		if(pElement->GetName() == GAME_TAG_PARAM_FOV)
		{
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pElement, temp, m_pOwnerManager->GetLogger()));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToFloat(temp, tempFloat));
			
			// Set the field-of-view
			//! @todo Is this always in degrees?
			m_pCamera->setFOVy(Ogre::Degree(static_cast<Ogre::Real>(tempFloat)));
	
		// Process position (?)
		} else if(pElement->GetName() == GAME_TAG_PARAM_PROJECTION) {

			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pElement, temp, m_pOwnerManager->GetLogger()));
			if(projectionType == GAME_TAG_VALUE_PERSPECTIVE)
			{
				m_pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
			} else if(projectionType == GAME_TAG_VALUE_ORTHOGRAPHIC) {
				m_pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
			} else {
				FWGLOG_WARNING_FORMAT(wxT("Unknown projection type value '%s' on line %d"), m_pOwnerManager->GetLogger()
													, temp.GetData().AsInternal()
													, pElement->GetLineNumber()
													, FWGLOG_ENDVAL);													
			}
	 
		// Process rotation (?)
		} else if(pElement->GetName() == GAME_TAG_PARAM_FAR) {
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pElement, temp, m_pOwnerManager->GetLogger()));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToFloat(temp, tempFloat));
			m_pCamera->setFarClipDistance(tempFloat);

		} else if(pElement->GetName() == GAME_TAG_PARAM_NEAR) {
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pElement, temp, m_pOwnerManager->GetLogger()));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToFloat(temp, tempFloat));
			
			m_pCamera->setNearClipDistance(tempFloat);
	 
		} else {
			GameXmlUtils::ProcessUnknownTag(pElement, m_pOwnerManager->GetLogger());
		}
		
		pElement = pElement->GetNext();	
	}
	
	
		
	return FWG_NO_ERROR;
}

void RenderCamera::Update()
{
}



GameErrorCode RenderCamera::Store(wxXmlNode* pParentNode)
{
	if(pParentNode == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	if(m_pCamera == nullptr)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	wxXmlNode *pNewNode = nullptr;
	wxXmlNode *pTempNode = nullptr;
	wxString content;
	Ogre::String contentOgre;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_RENDER_CAMERA));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	
	// get camera name
	contentOgre = m_pCamera->getName();
	pNewNode->AddAttribute(GAME_TAG_ATTR_NAME, wxString::FromUTF8(contentOgre.c_str()));
	
	// fov
	Ogre::Radian radian = m_pCamera->getFOVy();
	float tempFloat = static_cast<float>(radian.valueDegrees());
	FWG_RETURN_FAIL(GameXmlUtils::ConvertFromFloat( tempFloat, content));
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
								, pNewNode
								, wxXML_ELEMENT_NODE
								, GAME_TAG_PARAM_FOV
								, content));
	// projection
	switch(m_pCamera->getProjectionType())
	{
		case Ogre::PT_ORTHOGRAPHIC:
			content = GAME_TAG_VALUE_ORTHOGRAPHIC;
			break;
		case Ogre::PT_PERSPECTIVE:
		default:
			content = GAME_TAG_VALUE_PERSPECTIVE;
			break;
	}
	
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
								, pNewNode
								, wxXML_ELEMENT_NODE
								, GAME_TAG_PARAM_PROJECTION
								, content));
	// near
	tempFloat = static_cast<float>(m_pCamera->getNearClipDistance());
	FWG_RETURN_FAIL(GameXmlUtils::ConvertFromFloat( tempFloat, content));
	
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
								, pNewNode
								, wxXML_ELEMENT_NODE
								, GAME_TAG_PARAM_NEAR
								, content));
								
	// far
	tempFloat = static_cast<float>(m_pCamera->getFarClipDistance());
	FWG_RETURN_FAIL(GameXmlUtils::ConvertFromFloat( tempFloat, content));
	
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
								, pNewNode
								, wxXML_ELEMENT_NODE
								, GAME_TAG_PARAM_FAR
								, content));
								
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;	
	
}

