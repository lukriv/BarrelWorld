#include "glogicman.h"

#include <GameComp/gentity.h>
//#include "../InputComp/ginputcomp.h"


const Ogre::Real STEP_SIZE = 0.1f;

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
	
	if(m_pParent->GetInputComp() != nullptr)
	{
		FWG_RETURN_FAIL(ProcessInput());
	}
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicManualTest::ProcessInput()
{
	ControlStruct actualControls;
	m_pParent->GetInputComp()->ExportControlStruct(actualControls);
	
	if(m_pParent->GetTransformComp() != nullptr)
	{
		Ogre::SceneNode *pSceneNode = m_pParent->GetTransformComp()->GetSceneNode();
		Ogre::Vector3 moveVec = Ogre::Vector3::ZERO;
		if(actualControls.IsPressed(ControlStruct::INPUT_ACTION_UP))
		{
			moveVec.y += STEP_SIZE;
		}
		
		if(actualControls.IsPressed(ControlStruct::INPUT_ACTION_DOWN))
		{
			moveVec.y -= STEP_SIZE;
		}
		
		if(actualControls.IsPressed(ControlStruct::INPUT_ACTION_LEFT))
		{
			moveVec.x -= STEP_SIZE;
		}
		
		if(actualControls.IsPressed(ControlStruct::INPUT_ACTION_RIGHT))
		{
			moveVec.x += STEP_SIZE;
		}
		
		pSceneNode->translate(moveVec);
		
	}
	
	return FWG_NO_ERROR;
}
