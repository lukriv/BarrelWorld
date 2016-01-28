#include "gtranscomp.h"

#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <bullet/LinearMath/btAlignedAllocator.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>
#include <GameComp/gentitymgr.h>

typedef btAlignedAllocator< TransformData, 16 > TAllocator;

TransformComponent::TransformComponent() : ComponentBase(GAME_COMP_TRANSFORM)
										, m_pTransData(nullptr)
{}

GameErrorCode TransformComponent::Initialize()
{
	TAllocator allocator;
	m_pTransData = allocator.allocate(1);
	//FWG_RETURN_FAIL(GameNewChecked(m_pTransData));
	if(m_pTransData == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	m_pTransData->m_translate = btVector3(0.0f, 0.0f, 0.0f);
	m_pTransData->m_scale = btVector3(1.0f, 1.0f, 1.0f);
	m_pTransData->m_rotation = btQuaternion::getIdentity();
	
	return FWG_NO_ERROR;
}

TransformComponent::~TransformComponent()
{
	TAllocator allocator;
	if(m_pTransData != nullptr)
	{
		allocator.deallocate(m_pTransData);
		//GameDelete(m_pTransData);
		m_pTransData = nullptr;
	}
}

GameErrorCode TransformComponent::ReceiveMessage(TaskMessage& )
{
	return FWG_NO_ERROR;
}

void TransformComponent::getWorldTransform(btTransform& worldTrans) const
{
	if(m_pTransData)
	{
		btVector3 transform(m_pTransData->m_translate);
		btQuaternion rotation(m_pTransData->m_rotation);
							
		worldTrans.setOrigin(transform);
		worldTrans.setRotation(rotation);
	}
}

void TransformComponent::setWorldTransform(const btTransform& worldTrans)
{
	m_pTransData->m_rotation = worldTrans.getRotation();
	m_pTransData->m_translate = worldTrans.getOrigin();
	
	//transform was updated
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	GetEntityManager()->SendTaskMessage(GetComponentId(), task); 
}

GameErrorCode TransformComponent::Store(wxXmlNode* pParentNode)
{
	wxXmlNode *pNewNode = nullptr;
	wxXmlNode *pTempNode = nullptr;
	wxString content;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_TRANSFORM));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	
	// add position
	FWG_RETURN_FAIL(GameXmlUtils::ConvertFromVec3(cvt(m_pTransData->m_translate), content));
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
									, pNewNode
									, wxXML_ELEMENT_NODE
									, GAME_TAG_PARAM_POSITION
									, content));
	
	// add scale
	content.Clear();
	FWG_RETURN_FAIL(GameXmlUtils::ConvertFromVec3(cvt(m_pTransData->m_scale), content));
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
									, pNewNode
									, wxXML_ELEMENT_NODE
									, GAME_TAG_PARAM_SCALE
									, content));
									
	// add rotation
	content.Clear();
	FWG_RETURN_FAIL(GameXmlUtils::ConvertFromQuat(cvt(m_pTransData->m_rotation), content));
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
									, pNewNode
									, wxXML_ELEMENT_NODE
									, GAME_TAG_PARAM_ROTATION
									, content));
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
	
}


GameErrorCode TransformComponent::Load(wxXmlNode* pNode)
{
	if(pNode->GetName() != GAME_TAG_COMP_TRANSFORM)
	{
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	wxString nodeContent;
	wxXmlNode *pChild = pNode->GetChildren();
	
	while(pChild)
	{
		if(pChild->GetName() == GAME_TAG_PARAM_POSITION)
		{
			Ogre::Vector3 vec;
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pChild, nodeContent));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToVec3(nodeContent, vec));
			m_pTransData->m_translate = cvt(vec);
		} else if(pChild->GetName() == GAME_TAG_PARAM_SCALE) {
			Ogre::Vector3 vec;
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pChild, nodeContent));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToVec3(nodeContent, vec));
			m_pTransData->m_scale = cvt(vec);
		} else if(pChild->GetName() == GAME_TAG_PARAM_ROTATION) {
			Ogre::Quaternion quat;
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pChild, nodeContent));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToQuat(nodeContent, quat));
			m_pTransData->m_rotation = cvt(quat);
		} else {
			GameXmlUtils::ProcessUnknownTag(pChild, nullptr);
		}
		pChild = pChild->GetNext();
	}
	
	return FWG_NO_ERROR;
	
}

GameErrorCode TransformComponent::Create(const btVector3& position, const btVector3& scale, const btQuaternion& rotation)
{
	m_pTransData->m_translate = position;
	m_pTransData->m_scale = scale;
	m_pTransData->m_rotation = rotation;
	
	return FWG_NO_ERROR;
}


