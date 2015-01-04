#include "glogicman.h"

#include <GameComp/gentity.h>
#include <OGRE/OgrePrerequisites.h>
//#include "../InputComp/ginputcomp.h"


const float STEP_SIZE = 0.1f;

LogicManualTest::LogicManualTest()
{
}

LogicManualTest::~LogicManualTest()
{
}

GameErrorCode LogicManualTest::UserLogic()
{
	if(m_pParent == nullptr)
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
	ControlStruct actualControls;
	m_spInput->ExportControlStruct(actualControls);
	
	if(!m_spTransform.IsEmpty())
	{
		
		Vectormath::Aos::Vector3 moveVec = Vectormath::Aos::Vector3(0,0,0);
		if(actualControls.IsPressed(ControlStruct::INPUT_ACTION_UP))
		{
			moveVec.setZ(STEP_SIZE);
		}
		
		if(actualControls.IsPressed(ControlStruct::INPUT_ACTION_DOWN))
		{
			moveVec.setZ(-STEP_SIZE);
		}
		
		if(actualControls.IsPressed(ControlStruct::INPUT_ACTION_LEFT))
		{
			moveVec.setX(STEP_SIZE);
		}
		
		if(actualControls.IsPressed(ControlStruct::INPUT_ACTION_RIGHT))
		{
			moveVec.setX(-STEP_SIZE);
		}
		
		m_spTransform->GetData()->m_translate += moveVec;
		
	}
	
	// update render component
	ComponentBase* pRender = m_pParent->GetComponent(GAME_COMP_RENDER);
	if(pRender != nullptr) pRender->Update();
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicManualTest::ReceiveMessage(TaskMessage&)
{
	return FWG_NO_ERROR;
}

GameErrorCode LogicManualTest::ReinitComponent(GameEntity* pNewParentEntity)
{
	m_pParent = pNewParentEntity;
	if(pNewParentEntity)
	{
		m_spTransform = reinterpret_cast<TransformComponent*>(pNewParentEntity->GetComponent(GAME_COMP_TRANSFORM));
		m_spInput = reinterpret_cast<InputComponent*>(pNewParentEntity->GetComponent(GAME_COMP_INPUT));
		
		if(m_spTransform.IsEmpty() || m_spInput.IsEmpty())
		{
			return FWG_E_INVALID_PARAMETER_ERROR;
		}
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicManualTest::Update()
{
	return FWG_NO_ERROR;
}
