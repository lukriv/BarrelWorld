#include "glogicman.h"

#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include <OGRE/OgrePrerequisites.h>
#include <GameComp/gentitymgr.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>

#include <GameComp/LogicComp/glogicsystem.h>
#include "ginputchar.h"


const float STEP_SIZE = 0.1f;

LogicManualTest::LogicManualTest(){}

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



GameErrorCode LogicManualTest::Initialize(TransformComponent* pTransform, CharacterInput* pInput)
{
	if((pTransform == nullptr)||(pInput == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	m_spInput = pInput;
	
	return FWG_NO_ERROR;
}