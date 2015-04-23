#include "grenderrigidbody.h"

#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubMesh.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>
#include "grendercmgr.h"

RenderRigidBody::RenderRigidBody(RenderCompManager* pCompManager) : RenderComponent(GAME_COMP_RENDER_RIGID_BODY, pCompManager)
																, m_pEntity(nullptr)
{
}

RenderRigidBody::~RenderRigidBody()
{
	Clear();
}

GameErrorCode RenderRigidBody::Initialize(TransformComponent* pTransform)
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

void RenderRigidBody::ProcessUpdate()
{
    wxCriticalSectionLocker lock(m_renderLock);

    TMessageList::iterator iter;
    for(iter = m_receivedMessages.begin(); iter != m_receivedMessages.end(); ++iter) {
		switch(iter->GetTaskType()) 
		{
		case GAME_TASK_TRANSFORM_UPDATE:
			if(!m_spTransform.IsEmpty()) {
				// todo: upgrade updating scene node transform
				m_pSceneNode->setPosition(m_spTransform->GetOgreTranslate());
				m_pSceneNode->setOrientation(m_spTransform->GetOgreRotation());
			}
			break;
		default:
			break;
		}
    }

    // clear received messages
    m_receivedMessages.clear();

    // not yet in update queue
    m_alreadyInUpdateQueue = false;
}

GameErrorCode RenderRigidBody::Create(const wxString& meshName, const wxString& materialName)
{
	if(m_pEntity)
	{
		FWGLOG_ERROR(wxT("Component was already created!") , m_pOwnerManager->GetLogger());
		return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
	}
	
	m_pEntity = m_pOwnerManager->GetOgreSceneManager()->createEntity(meshName.ToStdString());
	if(m_pEntity == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	if(!materialName.IsEmpty())
	{
		m_pEntity->setMaterialName(materialName.ToStdString());
	} else {
		FWGLOG_WARNING(wxT("Render rigid body warning: Material should not be empty")
								, m_pOwnerManager->GetLogger());
	}
	
	m_pSceneNode->attachObject(m_pEntity);
	
	return FWG_NO_ERROR;
}

GameErrorCode RenderRigidBody::Load(wxXmlNode* pNode)
{
	if(m_pSceneNode == nullptr)
	{
		FWGLOG_ERROR(wxT("Object is not initialized"), m_pOwnerManager->GetLogger());
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(pNode->GetName() != GAME_TAG_COMP_RENDER_RIGID_BODY)
	{
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
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
	
	return Create(meshName, materialName);
}

GameErrorCode RenderRigidBody::Store(wxXmlNode* pParentNode)
{
	wxXmlNode *pNewNode = nullptr;
	wxXmlNode *pTempNode = nullptr;
	wxString content;
	Ogre::String contentOgre;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_RENDER_RIGID_BODY));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	
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


void RenderRigidBody::Clear()
{
	if(m_pEntity != nullptr)
	{
		m_pSceneNode->detachObject(m_pEntity);
		m_pOwnerManager->GetOgreSceneManager()->destroyEntity(m_pEntity);
		m_pEntity = nullptr;
	}
}
