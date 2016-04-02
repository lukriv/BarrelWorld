#include "charControl.h"

#include <GameComp/gcompmgr.h>
#include <GameComp/gentitymgr.h>
#include <GameComp/PhysicsComp/gphysutils.h>

const float MOVE_STEP_SIZE = 4.0f;
const float FALL_STEP_SIZE = 1.0f;

CharacterController::CharacterController(GameCompManager *pCompMgr, CharacterInput *pInput, PropertyComponent *pPropComp) : m_pCompMgr(pCompMgr)
	, m_spCharInput(pInput)
	, m_spPropComp(pPropComp)
	, m_diffVector(0,0,0)
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
		m_diffVector += move;
	}
	
	if(deltaTimeStep == 0.0f)
	{
		return;
	}
	
	btScalar distance = GamePhysicsUtils::ComputeGroundDistance(*m_pPhysSystem, *m_spKinematic);
	FWGLOG_INFO_FORMAT(wxT("distance from ground %.2f"), m_pPhysSystem->GetLogger(), distance, FWGLOG_ENDVAL);
	if(distance > 0.05f)
	{
		m_diffVector.setY(-FALL_STEP_SIZE);
	}
	
	m_diffVector *= (MOVE_STEP_SIZE*deltaTimeStep);
	m_spTransform->GetData()->m_rotation.setRotation(btVector3(0,1,0), GetActualAngle());
	
	m_spTransform->GetData()->m_translate += quatRotate(m_spTransform->GetData()->m_rotation,m_diffVector);
	
	
	
	// update render component
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	
	if(FWG_FAILED(result = GetEntityManager()->SendTaskMessage(GetComponentId(), task)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Update component failed: 0x%08x"), GetEntityManager()->GetLogger(), result, FWGLOG_ENDVAL);
	}
		
	m_diffVector.setValue(0,0,0);
}

btScalar CharacterController::GetActualAngle()
{
	btVector3 lookPoint;
	GetLookVector(lookPoint);
	
	lookPoint -= m_spTransform->GetData()->m_translate;
	
	return btAtan2Fast( -lookPoint.getX(), -lookPoint.getZ() );
}

void CharacterController::GetLookVector(btVector3 &lookPoint)
{
	Ogre::Vector2 vec2;
	m_pCompMgr->GetMenuSystem().GetPointerPosition(vec2);
	Ogre::Plane plane( Ogre::Vector3(0,1,0), 0);
	
	Ogre::Ray ray = m_pCompMgr->GetRenderSystem().GetMainCamera()->getCameraToViewportRay( vec2.x, vec2.y );
	
	std::pair< bool, Ogre::Real > rayResult = ray.intersects(plane);
	
	if(!rayResult.first)
	{
		return;
	}
	
	lookPoint = cvt(ray.getPoint(rayResult.second));
	
}


