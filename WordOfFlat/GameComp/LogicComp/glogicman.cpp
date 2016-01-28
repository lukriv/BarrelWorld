#include "glogicman.h"

#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <OGRE/OgrePrerequisites.h>
#include <GameComp/gentitymgr.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

#include "glogicsystem.h"
#include "../InputComp/ginputchar.h"
//#include "../InputComp/ginputcomp.h"


const float STEP_SIZE = 0.1f;

LogicManualTest::LogicManualTest()
{
}

LogicManualTest::~LogicManualTest()
{
}


GameErrorCode LogicManualTest::Update()
{
	if(m_spTransform.IsEmpty())
	{
		return FWG_E_OBJECT_NOT_INITIALIZED_ERROR;
	}
	
	if(!m_spInput.IsEmpty())
	{
		FWG_RETURN_FAIL(ProcessInput());
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicManualTest::ProcessInput()
{
	GameErrorCode result = FWG_NO_ERROR;
	ControlStruct actualControls;
	m_spInput->ExportControlStruct(actualControls);
	
	if(!m_spTransform.IsEmpty())
	{
		
		btVector3 moveVec = btVector3(0,0,0);
		if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_UP))
		{
			moveVec.setZ(STEP_SIZE);
		}
		
		if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_DOWN))
		{
			moveVec.setZ(-STEP_SIZE);
		}
		
		if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_LEFT))
		{
			moveVec.setX(STEP_SIZE);
		}
		
		if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_RIGHT))
		{
			moveVec.setX(-STEP_SIZE);
		}
		
		m_spTransform->GetData()->m_translate += moveVec;
		
	}
	
	// update render component
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	if(FWG_FAILED(result = GetEntityManager()->SendTaskMessage(GetComponentId(),task)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Update component failed: 0x%08x"), m_pLogicSystem->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicManualTest::ReceiveMessage(TaskMessage&)
{
	return FWG_NO_ERROR;
}


GameErrorCode LogicManualTest::Load(wxXmlNode* pNode)
{
	return GameXmlUtils::CheckTagAndType(pNode, GAME_TAG_COMP_MOVEABLE, GAME_TAG_TYPE_MOVEABLE_MANUAL_TEST, m_pLogicSystem->GetLogger());
}

GameErrorCode LogicManualTest::Store(wxXmlNode* pParentNode)
{
	wxXmlNode *pNewNode = nullptr;
	wxString content;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_MOVEABLE));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	pNewNode->AddAttribute(GAME_TAG_ATTR_TYPE, GAME_TAG_TYPE_MOVEABLE_MANUAL_TEST);
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}
