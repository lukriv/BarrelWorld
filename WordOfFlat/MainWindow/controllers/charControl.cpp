#include "charControl.h"

#include <GameComp/gentitymgr.h>

const float MOVE_STEP_SIZE = 4.0f;

CharacterController::CharacterController(CharacterInput *pInput) : m_spCharInput(pInput)
	, m_diffForwardBackward(0)
	, m_diffLeftRight(0)
{
}

CharacterController::~CharacterController()
{
}

void CharacterController::debugDraw(btIDebugDraw* debugDrawer)
{
}

void CharacterController::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep)
{
	GameErrorCode result = FWG_NO_ERROR;
	ControlStruct actualControls;
	m_spCharInput->ExportControlStruct(actualControls);
	
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_FORWARD))
	{
		m_diffForwardBackward -= MOVE_STEP_SIZE;
	}
	
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_BACKWARD))
	{
		m_diffForwardBackward += MOVE_STEP_SIZE;
	}
	
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_LEFT))
	{
		m_diffLeftRight -= MOVE_STEP_SIZE;
	}
	
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_RIGHT))
	{
		m_diffLeftRight += MOVE_STEP_SIZE;
	}
	
	if(deltaTimeStep == 0.0f)
	{
		return;
	}
	
	btVector3 divVec(m_diffLeftRight*deltaTimeStep,0,m_diffForwardBackward*deltaTimeStep);
	m_spTransform->GetData()->m_translate += divVec;
	
	
	// update render component
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	
	if(FWG_FAILED(result = GetEntityManager()->SendTaskMessage(GetComponentId(), task)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Update component failed: 0x%08x"), GetEntityManager()->GetLogger(), result, FWGLOG_ENDVAL);
	}
		
	m_diffForwardBackward = 0.0f;
	m_diffLeftRight = 0.0f;
}


