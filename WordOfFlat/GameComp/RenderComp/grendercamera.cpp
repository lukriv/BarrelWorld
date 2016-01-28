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
	wxString *pString = new (std::nothrow) wxString(cameraName);
	return m_pOwnerManager->CreateRenderComponent(this, pString);
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
	
	FWG_RETURN_FAIL(GameXmlUtils::CheckTagAndType(pNode, GAME_TAG_COMP_RENDER_OBJECT, GAME_TAG_TYPE_RENDER_CAMERA));
	
	wxString temp;
	float tempFloat = 0.0;
	
	wxString name;
	
	
	wxXmlNode *pElement = pNode->GetChildren();
	
	// find name tag
	while(pElement)
	{
		if(pElement->GetName() == GAME_TAG_PARAM_NAME)
		{
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pElement, name, m_pOwnerManager->GetLogger()));
		}
		
		pElement = pElement->GetNext();
	}
	
	
    wxString projectionType = GAME_TAG_VALUE_PERSPECTIVE;
 
    // Create the camera
    m_pCamera = m_pOwnerManager->GetOgreSceneManager()->createCamera(name.ToStdString());
    
	// attach camera to the node
	m_spPosition->GetSceneNode()->attachObject(m_pCamera);

 
	pElement = pNode->GetChildren();
 
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
			if(pElement->GetName() != GAME_TAG_PARAM_NEAR) // skip name tag
			{
				GameXmlUtils::ProcessUnknownTag(pElement, m_pOwnerManager->GetLogger());
			}
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
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_RENDER_OBJECT));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	pNewNode->AddAttribute(GAME_TAG_ATTR_TYPE, GAME_TAG_TYPE_RENDER_CAMERA);
	
	
	// get camera name
	contentOgre = m_pCamera->getName();
	
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
								, pNewNode
								, wxXML_ELEMENT_NODE
								, GAME_TAG_PARAM_NAME
								, contentOgre.c_str()));
	
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

void RenderCamera::OnCreation(void* pContext)
{
	wxString *pCameraName = reinterpret_cast<wxString*> (pContext);
	m_pCamera = m_pOwnerManager->GetOgreSceneManager()->createCamera(pCameraName->ToStdString());
	delete pCameraName;
}
