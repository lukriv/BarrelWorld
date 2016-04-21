#include "gphysbase.h"

#include "../gcompdefs.h"
#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

#include "../transformComp/gtranscomp.h"
#include "gphysshapeload.h"
#include "gphyssystem.h"

PhysicsBase::PhysicsBase(GamePhysicsSystem* pPhysSystem) : ComponentBase(GAME_COMP_PHYSICS), m_pPhysSystem(pPhysSystem), m_pRigidBody(nullptr)
{}

PhysicsBase::~PhysicsBase()
{
	if(m_pRigidBody) {
		m_pRigidBody->setUserPointer(nullptr);
		m_pPhysSystem->GetDynamicsWorld()->removeRigidBody(m_pRigidBody);
		delete m_pRigidBody;
		m_pRigidBody = nullptr;
	}
}

GameErrorCode PhysicsBase::Initialize(TransformComponent* pTransform)
{
	if(pTransform == nullptr) {
		FWGLOG_ERROR(wxT("Transform component is null"), m_pPhysSystem->GetLogger());
		return FWG_E_INVALID_PARAMETER_ERROR;
	}

	m_spTransform = pTransform;

	return FWG_NO_ERROR;
}

GameErrorCode PhysicsBase::Create(GamePropertyContainer& propMap)
{
	if(m_spTransform.IsEmpty()) {
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	GameErrorCode result = FWG_NO_ERROR;
	
	btScalar mass = 0;
	btCollisionShape* pColShape = nullptr;
	wxWord group = 1;
	wxWord mask = 0xffff;
	
	void * tempPtr = nullptr;
	if(FWG_FAILED(result = propMap.GetProperty(GAME_TAG_PARAM_PHYSICS_SHAPE, tempPtr)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Physics shape property '%s' was not found"), m_pPhysSystem->GetLogger()
			, GAME_TAG_PARAM_PHYSICS_SHAPE, FWGLOG_ENDVAL);
		return result;
	}
	
	pColShape = static_cast<btCollisionShape*>(tempPtr);
	
	if(FWG_FAILED(result = propMap.GetProperty(GAME_TAG_PARAM_MASS, static_cast<float&>(mass))))
	{
		FWGLOG_WARNING_FORMAT(wxT("Physics shape property '%s' was not found"), m_pPhysSystem->GetLogger()
			, GAME_TAG_PARAM_MASS, FWGLOG_ENDVAL);
	}
	
	if(FWG_FAILED(result = propMap.GetProperty(GAME_TAG_PARAM_GROUP, group)))
	{
		FWGLOG_INFO_FORMAT(wxT("Physics shape property '%s' was not found"), m_pPhysSystem->GetLogger()
			, GAME_TAG_PARAM_GROUP, FWGLOG_ENDVAL);
	}
	
	if(FWG_FAILED(result = propMap.GetProperty(GAME_TAG_PARAM_GROUP_MASK, mask)))
	{
		FWGLOG_INFO_FORMAT(wxT("Physics shape property '%s' was not found"), m_pPhysSystem->GetLogger()
			, GAME_TAG_PARAM_GROUP_MASK, FWGLOG_ENDVAL);
	}
	
	if(mass > 0) 
	{
		btVector3 localInertia;
		localInertia.setZero();

		pColShape->calculateLocalInertia(mass, localInertia);

		btRigidBody::btRigidBodyConstructionInfo info(mass, m_spTransform.In(), pColShape, localInertia);

		m_pRigidBody = new btRigidBody(info);
	} else {
		// create static object
		btRigidBody::btRigidBodyConstructionInfo info(0, m_spTransform.In(), pColShape, btVector3(0, 0, 0));
		m_pRigidBody = new btRigidBody(info);
	}

	if(m_pRigidBody == nullptr) 
	{
		return FWG_E_MEMORY_ALLOCATION_ERROR;
	}

	m_pPhysSystem->GetDynamicsWorld()->addRigidBody(m_pRigidBody, group, mask);

	// set pointer to this object
	m_pRigidBody->setUserPointer(static_cast<void*>(this));
	m_pRigidBody->setUserIndex(static_cast<int>(GetComponentId()));

	return FWG_NO_ERROR;
	
	
}

GameErrorCode PhysicsBase::Load(wxXmlNode* pNode)
{
	wxString tempContent;
	wxXmlNode *pChild = pNode->GetChildren();
	btCollisionShape *pCollShape = nullptr;
	float mass = 0.0f;

	while(pChild) {
		if(pChild->GetName() == GAME_TAG_PARAM_PHYSICS_SHAPE) {
			FWG_RETURN_FAIL(PhysicsShapeLoader::LoadShape(pChild, pCollShape, m_pPhysSystem->GetLogger()));
		} else if(pChild->GetName() == GAME_TAG_PARAM_MASS) {
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pChild, tempContent, m_pPhysSystem->GetLogger()));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToFloat(tempContent, mass));
		} else {
			GameXmlUtils::ProcessUnknownTag(pChild, m_pPhysSystem->GetLogger());
		}
		pChild = pChild->GetNext();
	}

	//return Create(static_cast<btScalar>(mass), pCollShape);
	
	return FWG_E_NOT_IMPLEMENTED_ERROR;
}

GameErrorCode PhysicsBase::Store(wxXmlNode* pParentNode)
{
	if(!m_pRigidBody) {
		return FWG_E_OBJECT_NOT_EXIST_ERROR;
	}

	GameErrorCode result = FWG_NO_ERROR;
	wxXmlNode *pNewNode = nullptr;
	wxXmlNode *pTempNode = nullptr;
	wxString content;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_PHYSICS));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);

	apNewNode->AddAttribute(GAME_TAG_ATTR_TYPE, GAME_TAG_TYPE_PHYSICS_RIGID_BODY);

	if(!m_definitionFile.IsEmpty()) {
		FWG_RETURN_FAIL(GameNewChecked(pTempNode
		                               , apNewNode.get()
		                               , wxXML_ELEMENT_NODE
		                               , GAME_TAG_PARAM_FILENAME
		                               , m_definitionFile));
	} else {

		if(m_pRigidBody->getCollisionShape()) {
			// store shape
			if(FWG_FAILED(result = PhysicsShapeLoader::StoreShape(apNewNode.get(), m_pRigidBody->getCollisionShape()))) {
				FWGLOG_ERROR_FORMAT(wxT("Store collision shape failed: 0x%08x"), m_pPhysSystem->GetLogger(), result, FWGLOG_ENDVAL);
				return result;
			}
		}

		FWG_RETURN_FAIL(GameXmlUtils::ConvertFromFloat( static_cast<float>(btScalar(1.0) / m_pRigidBody->getInvMass()) ,content));
		FWG_RETURN_FAIL(GameNewChecked(pTempNode
		                               , apNewNode.get()
		                               , wxXML_ELEMENT_NODE
		                               , GAME_TAG_PARAM_MASS
		                               , content));

	}

	pParentNode->AddChild(apNewNode.release());

	return FWG_NO_ERROR;
}

btRigidBody* PhysicsBase::GetRigidBody()
{
	return m_pRigidBody;
}

GameErrorCode PhysicsBase::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}


