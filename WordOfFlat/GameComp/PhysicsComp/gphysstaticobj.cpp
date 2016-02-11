#include "gphysstaticobj.h"

#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

#include "gphysshapeload.h"
#include "gphyssystem.h"
#include "../transformComp/gtranscomp.h"

PhysicsStaticObject::PhysicsStaticObject(GamePhysicsSystem* pPhysSystem) : PhysicsBase(pPhysSystem), m_pColObject(nullptr)
{}




PhysicsStaticObject::~PhysicsStaticObject()
{
	if(m_pColObject)
	{
		m_pColObject->setUserPointer(nullptr);
		m_pPhysSystem->GetDynamicsWorld()->removeCollisionObject(m_pColObject);
		delete m_pColObject;
		m_pColObject = nullptr;
	}
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
	
	m_pPhysSystem->GetDynamicsWorld()->addCollisionObject(m_pColObject);
	
	// set pointer to this object
	m_pColObject->setUserPointer(static_cast<void*>(this));
	
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
	FWG_RETURN_FAIL(GameXmlUtils::CheckTagAndType(pNode, GAME_TAG_COMP_PHYSICS, GAME_TAG_TYPE_PHYSICS_STATIC_OBJECT, m_pPhysSystem->GetLogger()));
	
	wxString tempContent;
	wxXmlNode *pChild = pNode->GetChildren();
	btCollisionShape *pCollShape = nullptr;
	
	while(pChild)
	{
		if(pChild->GetName() == GAME_TAG_PARAM_PHYSICS_SHAPE)
		{
			FWG_RETURN_FAIL(PhysicsShapeLoader::LoadShape(pChild, pCollShape, m_pPhysSystem->GetLogger()));
		} else {
			GameXmlUtils::ProcessUnknownTag(pChild, m_pPhysSystem->GetLogger());
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
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_TYPE_PHYSICS_STATIC_OBJECT));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	if(m_pColObject->getCollisionShape())
	{
		// store shape
		if(FWG_FAILED(result = PhysicsShapeLoader::StoreShape(pNewNode, m_pColObject->getCollisionShape(), m_pPhysSystem->GetLogger())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Store collision shape failed: 0x%08x"), m_pPhysSystem->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
	}
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}
