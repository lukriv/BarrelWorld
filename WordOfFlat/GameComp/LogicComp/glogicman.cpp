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

LogicManualTest::LogicManualTest(GameLogicSystem *pLogicSystem) : Moveable(pLogicSystem)
{
}

LogicManualTest::~LogicManualTest()
{
}


GameErrorCode LogicManualTest::Update(float timeDiff)
{
	GameErrorCode result = FWG_NO_ERROR;
	ControlStruct actualControls;
	m_spInput->ExportControlStruct(actualControls);
	
	float stepSize = STEP_SIZE*timeDiff;
	
	btVector3 moveVec = btVector3(0,0,0);
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_UP))
	{
		moveVec.setZ(stepSize);
	}
		
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_DOWN))
	{
		moveVec.setZ(-stepSize);
	}
		
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_LEFT))
	{
		moveVec.setX(stepSize);
	}
		
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_RIGHT))
	{
		moveVec.setX(-stepSize);
	}
		
	m_spTransform->GetData()->m_translate += moveVec;
		
	
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
