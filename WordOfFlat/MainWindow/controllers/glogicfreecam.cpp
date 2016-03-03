#include "glogicfreecam.h"
#include <GameComp/logicComp/glogicsystem.h>
#include <GameComp/gentitymgr.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>
#include <wx/xml/xml.h>
#include <wx/scopedptr.h>
#include "ginputfreecam.h"

const btScalar STEP_SIZE = 1.0f;
const btScalar MOVE_STEP_SIZE = 5.0f;

static const btScalar _2_PI = SIMD_2_PI;
static const btScalar _HALF_PI = SIMD_HALF_PI;

LogicFreeCamera::LogicFreeCamera() : LogicController()
	, m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_diffSinceLastFrameX(0.0f)
	, m_diffSinceLastFrameY(0.0f)
	, m_diffWheel(0.0f)
{
}

GameErrorCode LogicFreeCamera::Initialize(TransformComponent* pTransform, FreeCameraInput* pInput)
{
	
	if((pTransform == nullptr)||(pInput == nullptr))
	{
		return FWG_E_INVALID_PARAMETER_ERROR;
	}
	
	m_spTransform = pTransform;
	m_spInput = pInput;
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicFreeCamera::Update(float timeDiff)
{
	GameErrorCode result = FWG_NO_ERROR;
	ControlStruct actualControls;
	m_spInput->ExportControlStruct(actualControls);
	
	if(actualControls.IsMousePressed(ControlStruct::MOUSE_BUTTON_RIGHT))
	{
		m_diffSinceLastFrameX += (btScalar)actualControls.GetRelX();
		m_diffSinceLastFrameY += (btScalar)actualControls.GetRelY();
	}
	
	if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_FORWARD))
	{
		m_diffMoveVertical -= MOVE_STEP_SIZE;
	}
	
	if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_BACKWARD))
	{
		m_diffMoveVertical += MOVE_STEP_SIZE;
	}
	
	if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_LEFT))
	{
		m_diffMoveHorizontal -= MOVE_STEP_SIZE;
	}
	
	if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_RIGHT))
	{
		m_diffMoveHorizontal += MOVE_STEP_SIZE;
	}
	
	if(timeDiff == 0.0f)
	{
		return FWG_NO_ERROR;
	}
	
	if(!m_spTransform.IsEmpty())
	{
		
		// turn camera around
		m_angleX -= STEP_SIZE * (m_diffSinceLastFrameX) * timeDiff;
		m_angleY -= STEP_SIZE * (m_diffSinceLastFrameY) * timeDiff;
		
		if(m_angleX < 0)
		{
			m_angleX += _2_PI;
		} else if(m_angleX > _2_PI) {
			m_angleX -= _2_PI;				
		}
		
		if(m_angleY > _HALF_PI)
		{
			m_angleY = _HALF_PI;
		} else if(m_angleY < -_HALF_PI) {
			m_angleY = -_HALF_PI;
		}
		
		m_spTransform->GetData()->m_rotation.setEuler(m_angleX, m_angleY, 0.0f);
		
		{
			btVector3 dirVec(0,0, m_diffMoveVertical*timeDiff );
			m_spTransform->GetData()->m_translate += dirVec.rotate(m_spTransform->GetData()->m_rotation.getAxis(), m_spTransform->GetData()->m_rotation.getAngle());;
		}
		
		{
			btVector3 dirVec( m_diffMoveHorizontal*timeDiff, 0, 0);
			m_spTransform->GetData()->m_translate += dirVec.rotate(m_spTransform->GetData()->m_rotation.getAxis(), m_spTransform->GetData()->m_rotation.getAngle());;
		}
	}
	
	// update render component
	TaskMessage task(GAME_TASK_TRANSFORM_UPDATE);
	
	if(FWG_FAILED(result = GetEntityManager()->SendTaskMessage(GetComponentId(), task)))
	{
		FWGLOG_ERROR_FORMAT(wxT("Update component failed: 0x%08x"), m_pLogicSystem->GetLogger(), result, FWGLOG_ENDVAL);
		return result;
	}
	
	m_diffSinceLastFrameX = 0.0f;
	m_diffSinceLastFrameY = 0.0f;
	
	m_diffMoveVertical = 0.0f;
	m_diffMoveHorizontal = 0.0f;
	m_diffWheel = 0.0f;
	
	return FWG_NO_ERROR;
}

LogicFreeCamera::~LogicFreeCamera()
{
}
