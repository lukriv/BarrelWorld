#include "grenderrigidbody.h"

#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubMesh.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>
#include "grendersystem.h"


struct RenderRigidBodyContext {
	wxString meshName;
	wxString materialName;
};


RenderRigidBody::RenderRigidBody(GameRenderSystem* pCompManager) : RenderObject(pCompManager), m_pEntity(nullptr)
{
}

RenderRigidBody::~RenderRigidBody()
{
	Destroy();
}

void RenderRigidBody::Update()
{
    wxCriticalSectionLocker lock(m_renderLock);

    TMessageList::iterator iter;
    for(iter = m_receivedMessages.begin(); iter != m_receivedMessages.end(); ++iter) {
		switch(iter->GetTaskType()) 
		{
		default:
			break;
		}
    }

    // clear received messages
    m_receivedMessages.clear();

    // not yet in update queue
    m_alreadyInUpdateQueue = false;
}

GameErrorCode RenderRigidBody::Create(GamePropertyContainer &propMap)
{
	if(m_pEntity)
	{
		FWGLOG_ERROR(wxT("Component was already created!") , m_pOwnerManager->GetLogger());
		return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
	}
	
	GameErrorCode result = FWG_NO_ERROR;
	wxString str;
	bool tempBool = false;
	
	try 
	{

		if(FWG_FAILED(result = propMap.GetProperty(GAME_TAG_PARAM_MESH, str)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Mesh is missing : 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}

		m_pEntity = m_pOwnerManager->GetOgreSceneManager()->createEntity(str.ToStdString());
		if(m_pEntity == nullptr)
		{
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
		
		// material
		if(FWG_FAILED(result = propMap.GetProperty(GAME_TAG_PARAM_MATERIAL, str)))
		{
			FWGLOG_WARNING_FORMAT(wxT("Material is missing : 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		} else {
			m_pEntity->setMaterialName(str.ToStdString());
		}
		
		// shadows
		if(FWG_FAILED(result = propMap.GetProperty(GAME_TAG_PARAM_SHADOWS, tempBool)))
		{
			FWGLOG_WARNING_FORMAT(wxT("Shadows property is missing : 0x%08x"), m_pOwnerManager->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		} else {
			m_pEntity->setCastShadows(tempBool);
		}
		
		m_spPosition->GetSceneNode()->attachObject(m_pEntity);
		
	} catch (Ogre::Exception &exc) {
		
		FWGLOG_ERROR_FORMAT(wxT("%s"), m_pOwnerManager->GetLogger()
			, wxString::FromUTF8(exc.getFullDescription().c_str()).GetData().AsInternal()
			, FWGLOG_ENDVAL );
			
		return FWG_E_INVALID_DATA_ERROR;
	}
	
	return FWG_NO_ERROR;

}

GameErrorCode RenderRigidBody::Load(wxXmlNode* pNode)
{
	if(m_spPosition.IsEmpty())
	{
		FWGLOG_ERROR(wxT("Object is not initialized"), m_pOwnerManager->GetLogger());
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	FWG_RETURN_FAIL(GameXmlUtils::CheckTagAndType(pNode, GAME_TAG_COMP_RENDER_OBJECT, GAME_TAG_TYPE_RENDER_RIGID_BODY, m_pOwnerManager->GetLogger()));
	
	wxString meshName;
	wxString materialName;
	wxXmlNode *pChild = pNode->GetChildren();
	
	while(pChild)
	{
		if(pChild->GetName() == GAME_TAG_PARAM_MESH)
		{
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pChild, meshName));
		} else if(pChild->GetName() == GAME_TAG_PARAM_MATERIAL) {
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pChild, materialName));
		} else {
			GameXmlUtils::ProcessUnknownTag(pChild, m_pOwnerManager->GetLogger());
		}
		pChild = pChild->GetNext();
	}
	
	if(meshName.IsEmpty())
	{
		FWGLOG_ERROR_FORMAT(wxT("Failed load render rigid body: Mesh name not found - line %d")
								, m_pOwnerManager->GetLogger()
								, pNode->GetLineNumber()
								, FWGLOG_ENDVAL);
		return FWG_E_XML_TAG_NOT_FOUND_ERROR;
	}
	
	if(materialName.IsEmpty())
	{
		FWGLOG_WARNING_FORMAT(wxT("Render rigid body warning: Material name not found - line %d")
								, m_pOwnerManager->GetLogger()
								, pNode->GetLineNumber()
								, FWGLOG_ENDVAL);
	}
	
	//return Create(meshName, materialName);
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode RenderRigidBody::Store(wxXmlNode* pParentNode)
{
	wxXmlNode *pNewNode = nullptr;
	wxXmlNode *pTempNode = nullptr;
	wxString content;
	Ogre::String contentOgre;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_RENDER_OBJECT));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);

	pNewNode->AddAttribute(GAME_TAG_ATTR_TYPE, GAME_TAG_TYPE_RENDER_RIGID_BODY);
	
	// add mesh
	contentOgre = m_pEntity->getMesh()->getName();
	content.FromUTF8(contentOgre.c_str());
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
									, pNewNode
									, wxXML_ELEMENT_NODE
									, GAME_TAG_PARAM_MESH
									, content));
	
	Ogre::SubMesh *pSubMesh = m_pEntity->getMesh()->getSubMesh(0);
	// add material
	if(pSubMesh)
	{
		contentOgre.clear();
		content.Clear();
		
		contentOgre = pSubMesh->getMaterialName();
		content.FromUTF8(contentOgre.c_str());
		FWG_RETURN_FAIL(GameNewChecked(pTempNode
									, pNewNode
									, wxXML_ELEMENT_NODE
									, GAME_TAG_PARAM_MESH
									, content));
		
	} else {
		FWGLOG_WARNING(wxT("Ogre entity has no submesh"), m_pOwnerManager->GetLogger());
	}
	
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}


void RenderRigidBody::Destroy()
{
	if(m_pEntity != nullptr)
	{
		m_spPosition->GetSceneNode()->detachObject(m_pEntity);
		m_pOwnerManager->GetOgreSceneManager()->destroyEntity(m_pEntity);
		m_pEntity = nullptr;
	}
}

