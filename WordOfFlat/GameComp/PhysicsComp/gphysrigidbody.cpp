#include "gphysrigidbody.h"


#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>
#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

#include "../transformComp/gtranscomp.h"
#include "gphysshapeload.h"
#include "gphyssystem.h"


PhysicsRigidBody::PhysicsRigidBody(PhysicsSystem* pOwnerMgr)
    : PhysicsBase(pOwnerMgr), m_pRigidBody(nullptr)
{
}

PhysicsRigidBody::~PhysicsRigidBody()
{
	if(m_pRigidBody)
	{
		m_pRigidBody->setUserPointer(nullptr);
		m_pPhysSystem->GetDynamicsWorld()->removeRigidBody(m_pRigidBody);
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}
}

GameErrorCode PhysicsRigidBody::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsRigidBody::Update()
{
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsRigidBody::Create(btScalar mass, btCollisionShape* pColShape)
{
	if(m_spTransform.IsEmpty())
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	btVector3 localInertia;
	localInertia.setZero();
	
	pColShape->calculateLocalInertia(mass, localInertia);
	
	btRigidBody::btRigidBodyConstructionInfo info(mass, m_spTransform.In(), pColShape, localInertia);
	
	m_pRigidBody = new btRigidBody(info);
	if(m_pRigidBody == nullptr)
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}
	
	m_pPhysSystem->GetDynamicsWorld()->addRigidBody(m_pRigidBody);
	
	// set pointer to this object
	m_pRigidBody->setUserPointer(static_cast<void*>(this));
	
	return FWG_NO_ERROR;
	
}

GameErrorCode PhysicsRigidBody::Load(wxXmlNode* pNode)
{
	if(pNode->GetName() != GAME_TAG_COMP_PHYSICS_RIGID_BODY)
	{
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	wxString tempContent;
	wxXmlNode *pChild = pNode->GetChildren();
	btCollisionShape *pCollShape = nullptr;
	float mass = 0.0f;
	
	while(pChild)
	{
		if(pChild->GetName() == GAME_TAG_COMP_PHYSICS_SHAPE)
		{
			PhysicsShapeLoader shapeLoader(m_pPhysSystem->GetLogger());
			FWG_RETURN_FAIL(shapeLoader.LoadShape(pChild, pCollShape));
		} else if(pChild->GetName() == GAME_TAG_PARAM_MASS) {
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pChild, tempContent, m_pPhysSystem->GetLogger()));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToFloat(tempContent, mass));
		} else {
			GameXmlUtils::ProcessUnknownTag(pChild, m_pPhysSystem->GetLogger());
		}
		pChild = pChild->GetNext();
	}
	
	return Create(static_cast<btScalar>(mass), pCollShape);
}

GameErrorCode PhysicsRigidBody::Store(wxXmlNode* pParentNode)
{
	if(!m_pRigidBody)
	{
		return FWG_E_OBJECT_NOT_EXIST_ERROR;
	}
	
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode *pNewNode = nullptr;
	wxXmlNode *pTempNode = nullptr;
	wxString content;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_PHYSICS_RIGID_BODY));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	if(m_pRigidBody->getCollisionShape())
	{
		PhysicsShapeLoader shapeLoader(m_pPhysSystem->GetLogger());
		
		// store shape
		if(FWG_FAILED(result = shapeLoader.StoreShape(pNewNode, m_pRigidBody->getCollisionShape())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Store collision shape failed: 0x%08x"), m_pPhysSystem->GetLogger(), result, FWGLOG_ENDVAL);
			return result;
		}
	}
	
	FWG_RETURN_FAIL(GameXmlUtils::ConvertFromFloat( static_cast<float>(btScalar(1.0) / m_pRigidBody->getInvMass()) ,content));
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
									, pNewNode
									, wxXML_ELEMENT_NODE
									, GAME_TAG_PARAM_MASS
									, content));
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}
