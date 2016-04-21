#include "charControl.h"

#include <GameComp/gcompmgr.h>
#include <GameComp/gentitymgr.h>
#include <GameComp/gutils.h>
#include <GameComp/PhysicsComp/gphysutils.h>

const float MOVE_STEP_SIZE = 4.0f;
const float FALL_STEP_SIZE = 1.0f;
const float GROUND_TOLERANCE = 0.005f;

static const wxChar *FALLVELOCITY = wxT("fallVelocity");
static const wxChar *ONGROUND = wxT("onGround");

CharacterController::CharacterController(GameCompManager *pCompMgr, CharacterInput *pInput, PropertyComponent *pPropComp) : m_pCompMgr(pCompMgr)
	, m_spCharInput(pInput)
	, m_spPropComp(pPropComp)
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

	ControlStruct actualControls;
	m_spCharInput->ExportControlStruct(actualControls);

	if(actualControls.WasMouseClicked(ControlStruct::MOUSE_BUTTON_LEFT))
	{
		// do action
	}

	if(deltaTimeStep == 0.0f)
	{
		return;
	}
	
	GameErrorCode result = FWG_NO_ERROR;

	btVector3 move(0,0,0);
	
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_FORWARD))
	{
		move.setZ(-1.0f);
	}
	
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_BACKWARD))
	{
		move.setZ(move.getZ() + 1.0f);
	}
	
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_LEFT))
	{
		move.setX(-1.0f);
	}
	
	if(actualControls.IsPressed(CharacterInput::INPUT_ACTION_RIGHT))
	{
		move.setX(move.getX() + 1.0f);
	}
	
	if(!move.isZero())
	{
		move.normalize();
		move *= MOVE_STEP_SIZE;
	}
	

	
	btScalar distance = GamePhysicsUtils::ComputeGroundDistance(*m_pPhysSystem, *m_spKinematic);

	bool onGround = true;
	if(FWG_FAILED(result = m_spPropComp.In()->GetProperty(ONGROUND, onGround)))
	{
		FWGLOG_ERROR_FORMAT(wxT("%s property value does not exist: 0x%08x"), GetEntityManager()->GetLogger(), ONGROUND, result, FWGLOG_ENDVAL);
	}
	
	if(onGround)
	{
		move *= deltaTimeStep;
		if(distance < -GROUND_TOLERANCE)
		{
			move.setY(distance);
		}
		
		if(distance > GROUND_TOLERANCE)
		{
			m_spPropComp.In()->SetProperty(ONGROUND, false);
		}
		
		//FWGLOG_INFO_FORMAT(wxT("On ground - distance %.3f"), GetEntityManager()->GetLogger(), distance, result, FWGLOG_ENDVAL);

	} else {
		btVector3 fallVelocity(0,0,0);
		move.setZero();
		if(FWG_FAILED(result = m_spPropComp.In()->GetProperty(FALLVELOCITY, fallVelocity)))
		{
			FWGLOG_ERROR_FORMAT(wxT("%s property value does not exist: 0x%08x"), GetEntityManager()->GetLogger(), FALLVELOCITY, result, FWGLOG_ENDVAL);
		}

		if((distance < GROUND_TOLERANCE)&&(distance > -GROUND_TOLERANCE))
		{
			fallVelocity.setZero();
			m_spPropComp.In()->SetProperty(ONGROUND, true);
			m_spPropComp.In()->SetProperty(FALLVELOCITY, fallVelocity);
		} else if(distance > GROUND_TOLERANCE){
			
			move = (fallVelocity + m_pPhysSystem->GetDynamicsWorld()->getGravity()*(0.5*deltaTimeStep))*deltaTimeStep;
			fallVelocity += m_pPhysSystem->GetDynamicsWorld()->getGravity() * deltaTimeStep;
			m_spPropComp.In()->SetProperty(FALLVELOCITY, fallVelocity);
		} else {
			move.setY(-distance);
		}
	}
	


	
	m_spTransform->GetData()->m_rotation.setRotation(btVector3(0,1,0), GetActualAngle());
	
	m_spTransform->GetData()->m_translate += quatRotate(m_spTransform->GetData()->m_rotation, move);
	
	
	
	// update render component
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	
	if(FWG_FAILED(result = GetEntityManager()->SendTaskMessage(GetComponentId(), task)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Update component failed: 0x%08x"), GetEntityManager()->GetLogger(), result, FWGLOG_ENDVAL);
	}
		
}

btScalar CharacterController::GetActualAngle()
{
	btVector3 lookPoint;
	
	GameUtils::GetLookPoint(m_pCompMgr->GetRenderSystem(), m_pCompMgr->GetMenuSystem(), *m_spTransform, lookPoint);
	
	lookPoint -= m_spTransform->GetData()->m_translate;
	
	return btAtan2Fast( -lookPoint.getX(), -lookPoint.getZ() );
}




