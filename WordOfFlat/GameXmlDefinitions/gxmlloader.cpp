#include "gxmlloader.h"
#include "gxmldefs.h"
#include "gxmlkeydefs.h"
#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/arrstr.h> 
#include <GameSystem/new.h>
#include <GameComp/gcompmgr.h>
#include <GameComp/InputComp/ginputdef.h>
#include <GameComp/RenderComp/grenderrigidbody.h>
#include <GameComp/RenderComp/grendercamera.h>
#include <GameComp/PhysicsComp/gphysrigidbody.h>
#include "gxmlutils.h"
#include <GameComp/PhysicsComp/gphysstaticobj.h>


typedef GameBasMap<wxString, wxXmlNode*> TComponentMap;
	

GameErrorCode GameXmlResourceLoader::Initialize(const wxChar* xmlFileName, const wxChar* xmlFilePath, GameLogger* pLogger)
{
	wxFileName normFileName;
	
	if (m_isInitialized) {
		return FWG_NO_ERROR;
	}
	
	if (xmlFileName == nullptr)
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	if (xmlFilePath == nullptr)
	{
		normFileName.Assign(xmlFileName);
	} else {
		normFileName.Assign(xmlFilePath, xmlFileName);
	}
	
	m_fileName = normFileName.GetFullPath();

	m_spLogger = pLogger;

	m_isInitialized = true;
	return FWG_NO_ERROR;
}


///////////////////////////////////////////////////////////////////
///						LOAD									///
///////////////////////////////////////////////////////////////////

GameErrorCode GameXmlResourceLoader::Load(GameCompManager& compMgr)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxXmlDocument xmlDoc;
	
	wxXmlNode *pDefinitions = nullptr;
	wxXmlNode *pScene = nullptr;
	
	if(!m_isInitialized)
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(!xmlDoc.Load(m_fileName))
	{
		return FWG_E_OPEN_FILE_ERROR;
	}
	
	if(xmlDoc.GetRoot()->GetName() != GAME_TAG_ROOT_STR)
		return FWG_E_XML_ROOT_NOT_FOUND_ERROR;
	
	//get root
	wxXmlNode* child = xmlDoc.GetRoot()->GetChildren();
	while(child)
	{
		if(child->GetName() == GAME_TAG_DEFINITIONS_STR) 
		{
			pDefinitions = child;
		} else if(child->GetName() == GAME_TAG_SCENE_STR) {
			pScene = child;
		} else {
			// found unknown tag
			GameXmlUtils::ProcessUnknownTag(child, m_spLogger);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
	}
	
	if(pDefinitions)
	{
		if(FWG_FAILED(result = ParseDefinitions(pDefinitions, compMgr)))
		{
			FWGLOG_ERROR_FORMAT(wxT("ParseDefinitions failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No definitions found"), m_spLogger);
	}
	
	if(pScene)
	{
		if(FWG_FAILED(result = ParseScene(pScene, compMgr)))
		{
			FWGLOG_ERROR_FORMAT(wxT("ParseScene failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
	} else {
		FWGLOG_INFO(wxT("No scene found"), m_spLogger);
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::ParseDefinitions(wxXmlNode* pNode, GameCompManager& )
{
	
	wxXmlNode* child = pNode->GetChildren();
	while(child)
	{
		
		GameXmlUtils::ProcessUnknownTag(child,m_spLogger);
				
		child = child->GetNext();
	}
	
	
	return FWG_NO_ERROR;	
	
}

GameErrorCode GameXmlResourceLoader::ParseScene(wxXmlNode* pNode, GameCompManager& compMgr)
{
	GameErrorCode result = FWG_NO_ERROR;
	
	wxXmlNode *child = pNode->GetChildren();
	bool terrain = false;
	
	while(child)
	{
		if(child->GetName() == GAME_TAG_ENTITY_STR) 
		{
			
			// process entity definition
			if(FWG_FAILED(result = LoadEntity(child, compMgr)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Load entity failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			
		} else if(child->GetName() == GAME_TAG_COMP_TERRAIN) {
			if(terrain)
			{
				FWGLOG_ERROR_FORMAT(wxT("Tag '%s' already defined"), m_spLogger
										, child->GetName().GetData().AsInternal()
										, FWGLOG_ENDVAL);
				return FWG_E_XML_INVALID_TAG_ERROR;
			}

			if(FWG_FAILED(result = compMgr.GetTerrainManager().Load(child)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Load terrain failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			terrain = true;

		} else {
			// found unknown tag
			GameXmlUtils::ProcessUnknownTag(child, m_spLogger);
			return FWG_E_XML_UNKNOWN_TAG_ERROR;
		}
		
		child = child->GetNext();
	}
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadEntity(wxXmlNode* pNode, GameCompManager& compMgr)
{
	GameErrorCode result = FWG_NO_ERROR;
	TComponentMap components;
	wxDword entityId = 0;
	wxXmlNode *pComponent = nullptr;
	
	entityId = compMgr.GetEntityManager().GetNewId();
	
	pComponent = pNode->GetChildren();
	while (pComponent)
	{
		if(FWG_FAILED(result = components.Insert(pComponent->GetName(), pComponent)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Insert xml component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		
		pComponent = pComponent->GetNext();
	}
	
	// process components
	// tranform
	pComponent = components.Exists(GAME_TAG_COMP_TRANSFORM)? *components.FindValue(GAME_TAG_COMP_TRANSFORM) : nullptr;
	if(pComponent)
	{
		FWG_RETURN_FAIL(LoadTransform(pComponent, compMgr, entityId));
	}
	
	
	// input
	pComponent = components.Exists(GAME_TAG_COMP_INPUT)? *components.FindValue(GAME_TAG_COMP_INPUT) : nullptr;
	if(pComponent)
	{
		FWG_RETURN_FAIL(LoadInput(pComponent, compMgr, entityId));
	}
	
	// render 
	pComponent = components.Exists(GAME_TAG_COMP_RENDER_OBJECT)? *components.FindValue(GAME_TAG_COMP_RENDER_OBJECT) : nullptr;
	if(pComponent)
	{
		FWG_RETURN_FAIL(LoadRender(pComponent, compMgr, entityId));
	}
	
	// moveable
	pComponent = components.Exists(GAME_TAG_COMP_MOVEABLE)? *components.FindValue(GAME_TAG_COMP_MOVEABLE) : nullptr;
	if(pComponent)
	{
		FWG_RETURN_FAIL(LoadMoveable(pComponent, compMgr, entityId))
	}
	
	// physics
	pComponent = components.Exists(GAME_TAG_COMP_PHYSICS)? *components.FindValue(GAME_TAG_COMP_PHYSICS) : nullptr;
	if(pComponent)
	{
		FWG_RETURN_FAIL(LoadPhysics(pComponent, compMgr, entityId))
	}
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadTransform(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<TransformComponent> spTransform;
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetTransformManager().CreateComponent(entID, spTransform.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = spTransform->Load(pNode)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load transform component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}

GameErrorCode GameXmlResourceLoader::LoadInput(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<InputComponent> spInput;
	RefObjSmPtr<InputDef> spInputDefinition;
	wxString type;
	
	if(!pNode->GetAttribute(GAME_TAG_ATTR_TYPE, &type))
	{
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// continue
	if(type == GAME_TAG_TYPE_INPUT_CHARACTER)
	{
		CharacterInput *pInput = nullptr;
		if(FWG_FAILED(result = compMgr.GetEntityManager().GetInputManager().CreateComponent<CharacterInput>(entID, pInput)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Create character input component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}	
		spInput.Attach(pInput);
	} else if(type == GAME_TAG_TYPE_INPUT_FREE_CAMERA) {
		FreeCameraInput *pInput = nullptr;
		if(FWG_FAILED(result = compMgr.GetEntityManager().GetInputManager().CreateComponent<FreeCameraInput>(entID, pInput)))
		{
			FWGLOG_ERROR_FORMAT(wxT("Create free camera component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		spInput.Attach(pInput);
	}
	
	if(spInput.IsEmpty())
	{
		result = FWG_E_INVALID_DATA_ERROR;
		FWGLOG_ERROR_FORMAT(wxT("Unknown input type or create input component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = CreateInputDef(pNode, spInputDefinition)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create input definition failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = spInput->Create(spInputDefinition)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create input component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}

GameErrorCode GameXmlResourceLoader::CreateInputDef(wxXmlNode* pNode, RefObjSmPtr<InputDef> &spInputDef)
{
	GameErrorCode result = FWG_NO_ERROR;
	wxInt32 keyCode = 0;
	wxString action;
	wxString typeStr;
	RefObjSmPtr<InputDef> spTempInput;
	
	wxXmlNode *child = pNode->GetChildren();

	
	// create new input component
	FWG_RETURN_FAIL(GameNewChecked(spTempInput.OutRef()));
	
	while (child)
	{
		if(child->GetName() == GAME_TAG_PARAM_INPUT_KEY) 
		{
			keyCode = 0;
			action.Clear(); // clear tempValue for sure
			
			if(FWG_FAILED(result = GameXmlUtils::GetKeyValue(child, action, keyCode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Read key attributes failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
				return result;
			}
			
			if(FWG_FAILED(result = spTempInput->m_inputMap.Insert(action, keyCode)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Insert key action failed (action='%s' on line='%d'): 0x%08x"), m_spLogger
										, action.GetData().AsInternal(), child->GetLineNumber(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			GameXmlUtils::ProcessUnknownTag(child, m_spLogger);
		}
		
		child = child->GetNext();
		
	}
	
	
	// set return value
	spInputDef = spTempInput;
	
	
	return FWG_NO_ERROR;
}

GameErrorCode GameXmlResourceLoader::LoadRender(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<RenderPosition> spRenderPos;
	RefObjSmPtr<RenderObject> spRenderObj;
	wxString type;
	
	RefObjSmPtr<TransformComponent> spTransform = compMgr.GetEntityManager().GetTransformManager().GetComponent(entID);
	
	// create render pos
	if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderPosManager().CreateComponent(entID, spTransform, spRenderPos.OutRef())))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render position component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	if(FWG_FAILED(result = spRenderPos->Create()))
	{
		FWGLOG_ERROR_FORMAT(wxT("Create render position failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	// get type of object
	if(!pNode->GetAttribute(GAME_TAG_ATTR_TYPE, &type))
	{
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// create render object
	if(type == GAME_TAG_TYPE_RENDER_RIGID_BODY)
	{
		RefObjSmPtr<RenderRigidBody> spTemp;
		if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderObjManager().CreateComponent<RenderRigidBody>(entID, spRenderPos, spTemp.OutRef())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Create character input component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		spRenderObj = spTemp;
	} else if(type == GAME_TAG_TYPE_RENDER_CAMERA) {
		RefObjSmPtr<RenderCamera> spTemp;
		if(FWG_FAILED(result = compMgr.GetEntityManager().GetRenderObjManager().CreateComponent<RenderCamera>(entID, spRenderPos, spTemp.OutRef())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Create free camera component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		spRenderObj = spTemp;	
	} else {
		FWGLOG_ERROR_FORMAT(wxT("Unknown robject component type found: %s"), m_spLogger, type.GetData().AsInternal(), FWGLOG_ENDVAL);
		return FWG_E_INVALID_DATA_ERROR;
	}
	
	if(FWG_FAILED(result = spRenderObj->Load(pNode)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load render object failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
		
	return result;
}

GameErrorCode GameXmlResourceLoader::LoadMoveable(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<ComponentBase> spLogic;
	
	wxString type;
	
	RefObjSmPtr<TransformComponent> spTransform = compMgr.GetEntityManager().GetTransformManager().GetComponent(entID);
	
	RefObjSmPtr<InputComponent> spInput = compMgr.GetEntityManager().GetInputManager().GetComponent(entID);
	
	// get type of object
	if(!pNode->GetAttribute(GAME_TAG_ATTR_TYPE, &type))
	{
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// create logic
	if(type == GAME_TAG_TYPE_MOVEABLE_MANUAL_TEST)
	{
		RefObjSmPtr<LogicManualTest> spTemp;
		if(FWG_FAILED(result = compMgr.GetEntityManager().GetMoveableManager().CreateComponent<LogicManualTest>(entID, spTransform, spInput, spTemp.OutRef())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Create character moveable component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}	
		spLogic = spTemp;
	} else if(type == GAME_TAG_TYPE_MOVEABLE_FREE_CAMERA) {
		RefObjSmPtr<LogicFreeCamera> spTemp;
		if(FWG_FAILED(result = compMgr.GetEntityManager().GetMoveableManager().CreateComponent<LogicFreeCamera>(entID, spTransform, spInput, spTemp.OutRef())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Create free camera moveable component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		spLogic = spTemp;	
	} else {
		FWGLOG_ERROR_FORMAT(wxT("Unknown moveable component type found: %s"), m_spLogger, type.GetData().AsInternal(), FWGLOG_ENDVAL);
		return FWG_E_INVALID_DATA_ERROR;
	}
	
	if(FWG_FAILED(result = spLogic->Load(pNode)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load moveable component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}

GameErrorCode GameXmlResourceLoader::LoadPhysics(wxXmlNode* pNode, GameCompManager& compMgr, wxDword entID)
{
	GameErrorCode result = FWG_NO_ERROR;
	RefObjSmPtr<PhysicsBase> spPhysics;
	
	wxString type;
	
	RefObjSmPtr<TransformComponent> spTransform = compMgr.GetEntityManager().GetTransformManager().GetComponent(entID);
	
	// get type of object
	if(!pNode->GetAttribute(GAME_TAG_ATTR_TYPE, &type))
	{
		return FWG_E_XML_ATTR_NOT_FOUND_ERROR;
	}
	
	// create logic
	if(type == GAME_TAG_TYPE_PHYSICS_RIGID_BODY)
	{
		RefObjSmPtr<PhysicsRigidBody> spTemp;
		if(FWG_FAILED(result = compMgr.GetEntityManager().GetPhysicsManager().CreateComponent<PhysicsRigidBody>(entID, spTransform, spTemp.OutRef())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Create physics rigid body component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		spPhysics = spTemp;
	} else if(type == GAME_TAG_TYPE_PHYSICS_STATIC_OBJECT) {
		RefObjSmPtr<PhysicsStaticObject> spTemp;
		if(FWG_FAILED(result = compMgr.GetEntityManager().GetPhysicsManager().CreateComponent<PhysicsStaticObject>(entID, spTransform, spTemp.OutRef())))
		{
			FWGLOG_ERROR_FORMAT(wxT("Create physics static component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
			return result;
		}
		spPhysics = spTemp;	
	}
	
	if(FWG_FAILED(result = spPhysics->Load(pNode)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Load physics component failed: 0x%08x"), m_spLogger, result, FWGLOG_ENDVAL);
		return result;
	}
	
	return result;
}
