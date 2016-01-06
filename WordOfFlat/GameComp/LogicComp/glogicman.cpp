#include "glogicman.h"

#include <GameComp/gentity.h>
#include <OGRE/OgrePrerequisites.h>

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

GameErrorCode LogicManualTest::Initialize(TransformComponent* pTransform, InputComponent *pInput)
{
	if((pTransform == nullptr)||(pInput == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	m_spInput = pInput;
		
	return FWG_NO_ERROR;
}

GameErrorCode LogicManualTest::UserLogic()
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
	if(FWG_FAILED(result = GetParentEntity()->ReceiveMessage(task)))
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
	return FWG_NO_ERROR;
}

GameErrorCode LogicManualTest::Store(wxXmlNode* pParentNode)
{
	return FWG_NO_ERROR;
}
