#include "gphysshapeload.h"

#include <wx/string.h>
#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <bullet/BulletCollision/CollisionShapes/btCollisionShape.h>
#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>
#include <GameXmlDefinitions/gxmlutils.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameSystem/new.h>
#include <OGRE/OgreVector3.h>
#include "gogrebulletutils.h"



GameErrorCode PhysicsShapeLoader::LoadShape(wxXmlNode* pNode, btCollisionShape*& pCollisionShape, GameLogger *pLogger)
{
	
	if(pNode->GetName() != GAME_TAG_PARAM_PHYSICS_SHAPE)
	{
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	wxXmlNode *pChild = pNode->GetChildren();
	
	if(pChild)
	{
		FWG_RETURN_FAIL(LoadOneShape(pChild, pCollisionShape, pLogger));
		pChild = pChild->GetNext();
	}
	
	if(pChild == nullptr)
	{
		// only one shape 
		return FWG_NO_ERROR;
	}
	
	// todo: prepare btCompoundShape
	
	
	while(pChild)
	{
		// todo: load shapes and add it to compound shape
		pChild = pChild->GetNext();
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsShapeLoader::LoadOneShape(wxXmlNode* pNode, btCollisionShape*& pCollisionShape, GameLogger *pLogger)
{
	if(pNode->GetName() == GAME_TAG_PARAM_PHYSICS_BOX_SHAPE)
	{
		return LoadBox(pNode, pCollisionShape, pLogger);
	} else {
		GameXmlUtils::ProcessUnknownTag(pNode, pLogger);
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsShapeLoader::StoreShape(wxXmlNode* pParentNode, btCollisionShape* pCollisionShape, GameLogger *pLogger)
{
	wxXmlNode *pNewNode = nullptr;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_PARAM_PHYSICS_SHAPE));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	switch(pCollisionShape->getShapeType())
	{
	case BOX_SHAPE_PROXYTYPE:
		FWG_RETURN_FAIL(StoreBox(pNewNode, reinterpret_cast<btBoxShape*>(pCollisionShape)));
	default:
		return FWG_E_NOT_IMPLEMENTED_ERROR;		
	}
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsShapeLoader::LoadBox(wxXmlNode* pNode, btCollisionShape*& pCollisionShape, GameLogger *pLogger)
{
	if(pNode->GetName() != GAME_TAG_PARAM_PHYSICS_BOX_SHAPE)
	{
		return FWG_E_XML_INVALID_TAG_ERROR;
	}
	
	wxString tempStr;
	Ogre::Vector3 boxHalfSize = Ogre::Vector3::ZERO;
	wxXmlNode *pChild = pNode->GetChildren();
	

	while(pChild)
	{
		if(pChild->GetName() == GAME_TAG_PARAM_BOX_HALF_SIZE)
		{
			FWG_RETURN_FAIL(GameXmlUtils::GetNodeContent(pChild, tempStr, pLogger));
			FWG_RETURN_FAIL(GameXmlUtils::ConvertToVec3(tempStr, boxHalfSize));
		} else {
			GameXmlUtils::ProcessUnknownTag(pChild, pLogger);
		}
		pChild = pChild->GetNext();
	}
	
	pCollisionShape = new btBoxShape(cvt(boxHalfSize));
	
	return FWG_NO_ERROR;
}

GameErrorCode PhysicsShapeLoader::StoreBox(wxXmlNode* pParentNode, btBoxShape* pCollisionShape, GameLogger *pLogger)
{
	wxXmlNode *pNewNode = nullptr;
	wxXmlNode *pTempNode = nullptr;
	wxString content;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_PARAM_PHYSICS_BOX_SHAPE));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	
	// add mesh
	FWG_RETURN_FAIL(GameXmlUtils::ConvertFromVec3( cvt(pCollisionShape->getHalfExtentsWithoutMargin()), content));
		
	FWG_RETURN_FAIL(GameNewChecked(pTempNode
									, pNewNode
									, wxXML_ELEMENT_NODE
									, GAME_TAG_PARAM_BOX_HALF_SIZE
									, content));
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}




