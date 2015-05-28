#include "gphysstaticobj.h"

#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

#include "gphysshapeload.h"
#include "gphyscmgr.h"
#include "../transformComp/gtranscomp.h"

PhysicsStaticObject::PhysicsStaticObject(PhysicsCompManager* pOwnerMgr) : ComponentBase(GAME_COMP_PHYSICS_STATIC_OBJECT)
				, m_pOwnerMgr(pOwnerMgr)
				, m_pColObject(nullptr)
{}




PhysicsStaticObject::~PhysicsStaticObject()
{
	if(m_pColObject)
	{
		m_pColObject->setUserPointer(nullptr);
		m_pOwnerMgr->GetDynamicsWorld()->removeCollisionObject(m_pColObject);
		delete m_pColObject;
		m_pColObject = nullptr;
	}
}


GameErrorCode PhysicsStaticObject::Initialize(TransformComponent* pTransform)
{
	if (pTransform == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	
	return FWG_NO_ERROR;
}


GameErrorCode PhysicsStaticObject::Create(btCollisionShape *pColShape)
{
	if(m_spTransform.IsEmpty())
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	m_pColObject = new btCollisionObject;
	btTransform transform;
	m_spTransform->getWorldTransform(transform);
	m_pColObject->setWorldTransform(transform);
	m_pColObject->setCollisionShape(pColShape);
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsStaticObject::Update()
{
	return FWG_NO_ERROR;
}


GameErrorCode PhysicsStaticObject::ReceiveMessage(TaskMessage& msg)
{
	switch(msg.GetTaskType())
	{
		case GAME_TASK_TRANSFORM_UPDATE:
		{
			btTransform transform;
			m_spTransform->getWorldTransform(transform);
			m_pColObject->setWorldTransform(transform);
			//FWGLOG_DEBUG_FORMAT(wxT("Update physics for entity: %s"), m_pOwnerMgr->GetLogger(),
			//													m_pParent->GetName().GetData().AsInternal(),
			//													FWGLOG_ENDVAL);
			break;
		}
		default: 
			break;
	}
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsStaticObject::Load(wxXmlNode* pNode)
{
	if(pNode->GetName() != GAME_TAG_COMP_PHYSICS_STATIC_OBJECT)
	{
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	wxString tempContent;
	wxXmlNode *pChild = pNode->GetChildren();
	btCollisionShape *pCollShape = nullptr;
	
	while(pChild)
	{
		if(pChild->GetName() == GAME_TAG_COMP_PHYSICS_SHAPE)
		{
			PhysicsShapeLoader shapeLoader(m_pOwnerMgr->GetLogger());
			FWG_RETURN_FAIL(shapeLoader.LoadShape(pChild, pCollShape));
		} else {
			GameXmlUtils::ProcessUnknownTag(pChild, m_pOwnerMgr->GetLogger());
		}
		pChild = pChild->GetNext();
	}
	
	return Create( pCollShape);
}

GameErrorCode PhysicsStaticObject::Store(wxXmlNode* pParentNode)
{
	if(!m_pColObject)
	{
		return FWG_E_OBJECT_NOT_EXIST_ERROR;
	}
	
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode *pNewNode = nullptr;
	//wxXmlNode *pTempNode = nullptr;
	wxString content;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_PHYSICS_STATIC_OBJECT));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	if(m_pColObject->getCollisionShape())
	{
		PhysicsShapeLoader shapeLoader(m_pOwnerMgr->GetLogger());
		
		// store shape
		if(FWG_FAILED(result = shapeLoader.StoreShape(pNewNode, m_pColObject->getCollisionShape())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Store collision shape failed: 0x%08x"), m_pOwnerMgr->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
	}
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}
