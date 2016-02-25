#include "glogicgamecam.h"
#include "glogicsystem.h"
#include <GameComp/inputComp/ginputfreecam.h>
#include <GameComp/gentitymgr.h>
#include <GameXmlDefinitions/gxmldefs.h>
#include <GameXmlDefinitions/gxmlutils.h>
#include <wx/xml/xml.h>
#include <wx/scopedptr.h>

const btScalar STEP_SIZE = 1.0f;
const btScalar MOVE_STEP_SIZE = 5.0f;

static const btScalar _2_PI = SIMD_2_PI;
static const btScalar _HALF_PI = SIMD_HALF_PI;

LogicGameCamera::LogicGameCamera(GameLogicSystem *pLogicSystem) : Moveable(pLogicSystem)
	, m_angleX(0.0f)
	, m_angleY(0.0f)
	, m_diffSinceLastFrameX(0.0f)
	, m_diffSinceLastFrameY(0.0f)
	, m_diffMoveVertical(0.0f)
	, m_diffMoveHorizontal(0.0f)
{
}

GameErrorCode LogicGameCamera::ReceiveMessage(TaskMessage& msg)
{
	return FWG_NO_ERROR;
}


GameErrorCode LogicGameCamera::Update(float timeDiff)
{
	GameErrorCode result = FWG_NO_ERROR;
	ControlStruct actualControls;
	m_spInput->ExportControlStruct(actualControls);
	
	if(actualControls.IsMousePressed(ControlStruct::MOUSE_BUTTON_RIGHT))
	{
		m_diffSinceLastFrameX += (btScalar)actualControls.GetRelX();
		m_diffSinceLastFrameY += (btScalar)actualControls.GetRelY();
	}
	
	if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_UP))
	{
		m_diffMoveVertical -= MOVE_STEP_SIZE;
	}
	
	if(actualControls.IsPressed(FreeCameraInput::INPUT_ACTION_DOWN))
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
			btVector3 dirVec(0,0,m_diffMoveVertical*timeDiff );
			m_spTransform->GetData()->m_translate += dirVec;
		}
		
		{
			btVector3 dirVec(  m_diffMoveHorizontal*timeDiff , 0, 0);
			m_spTransform->GetData()->m_translate += dirVec;
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
	
	return FWG_NO_ERROR;
}

GameErrorCode LogicGameCamera::Load(wxXmlNode* pNode)
{
	return GameXmlUtils::CheckTagAndType(pNode, GAME_TAG_COMP_MOVEABLE, GAME_TAG_TYPE_MOVEABLE_FREE_CAMERA, m_pLogicSystem->GetLogger());
	
}

GameErrorCode LogicGameCamera::Store(wxXmlNode* pParentNode)
{
	wxXmlNode *pNewNode = nullptr;
	wxString content;
	FWG_RETURN_FAIL(GameNewChecked(pNewNode, wxXML_ELEMENT_NODE, GAME_TAG_COMP_MOVEABLE));
	wxScopedPtr<wxXmlNode> apNewNode(pNewNode);
	
	pNewNode->AddAttribute(GAME_TAG_ATTR_TYPE, GAME_TAG_TYPE_MOVEABLE_FREE_CAMERA);
	
	pParentNode->AddChild(apNewNode.release());
	
	return FWG_NO_ERROR;
}

LogicGameCamera::~LogicGameCamera()
{
}
